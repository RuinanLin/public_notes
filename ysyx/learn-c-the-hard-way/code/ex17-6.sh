#!/bin/bash
set -e

# Configuration
DB_FILE="db.dat"
SHADOW_DB="shadow_records.txt"	# Shadow database, recording IDs that we believe should exist
EXE="./build/ex17"
ITERATIONS=100	# Run how many iterations of test
MAX_ID=99	# MAX_ROWS-1 in the program

# Initialization
rm -f $DB_FILE $SHADOW_DB
touch $SHADOW_DB
$EXE $DB_FILE c
echo "--- Start testing ($ITERATIONS iterations in total) ---"

for ((i=1; i<=ITERATIONS; i++))
do
	# Randomly select an operation: 1=set, 2=get, 3=delete
	ACTION=$((RANDOM % 3 + 1))
	TARGET_ID=$((RANDOM % (MAX_ID + 1)))

	case $ACTION in
		1)	# SET operation
			NAME="User_$RANDOM"
			EMAIL="mail_$RAMDOM@test.com"

			# If ID is already in the shadow database, our program will give an error, so first judge
			if grep -q "^$TARGET_ID$" "$SHADOW_DB"; then
				# Delete and set again (or we may also skip this test)
				$EXE $DB_FILE d $TARGET_ID > /dev/null
			fi

			$EXE $DB_FILE s $TARGET_ID "$NAME" "$EMAIL" > /dev/null
			echo "$TARGET_ID" >> "$SHADOW_DB"
			echo "Round $i: [SET]    ID $TARGET_ID"
			;;

		2)	# GET operation
			echo -n "Round $i: [GET]    ID $TARGET_ID -> "
			if grep -q "^$TARGET_ID$" "$SHADOW_DB"; then
				# It is set
				$EXE $DB_FILE g $TARGET_ID | grep -q "$TARGET_ID" && echo "SUCCESS"
			else
				# It is not set
				set +e
				RESULT=$($EXE $DB_FILE g $TARGET_ID 2>&1)
				if [[ $RESULT == *"ERROR: ID is not set"* ]]; then
					echo "PASS (Expected Error)"
				else
					echo "FAIL (Should have errored)"
					exit 1
				fi
				set -e
			fi
			;;

		3)	# DELETE operation
			$EXE $DB_FILE d $TARGET_ID > /dev/null
			# Remove this ID from the shadow database
			sed -i "/^$TARGET_ID$/d" "$SHADOW_DB"
			echo "Round $i: [DELETE] ID $TARGET_ID"
			;;
	esac

	# Synchronization question for writing shadow database
	sort -u -o "$SHADOW_DB" "$SHADOW_DB"
done

echo "--- Random operations completed. Now start to check LIST ----"
# Check the numbers
SHADOW_COUNT=$(wc -l < "$SHADOW_DB")
ACTUAL_COUNT=$($EXE $DB_FILE l | wc -l)

if [ "$SHADOW_COUNT" -eq "$ACTUAL_COUNT" ]; then
	echo "PERFECT"
else
	echo "FAILED. SHADOW_COUNT: $SHADOW_COUNT, ACTUAL_COUNT: $ACTUAL_COUNT"
	exit 1
fi

echo "--- [Cong] Your program has passed all tests !!! ---"
