| Syntax | Description |
| --- | --- |
| Header | Title |
| Paragraph | Text |

| Col 1 | Col 2     | Col 3 |       Col 4      |
|-------|-----------|-------|:----------------:|
| hello | this      | is    |         a        |
| table | generated | by    | [Tables Generator][1] |

[1]: <https://www.tablesgenerator.com/markdown_tables> "Tables Generator"

| Syntax      | Description | Test Text     |
| :---        |    :----:   |          ---: |
| Header      | Title       | Here's this   |
| Paragraph   | Text        | And more      |

| Syntax            | Description | Test Text     |
| :---              |    :----:   |          ---: |
| Header            | Title       | Here's this   |
| Para&#124;graph   | Text        | And more      |

```
{
	"firstName": "John",
	"lastName": "Smith",
	"age": 25
}
```

````
{
	"firstName": "John",
	"lastName": "Smith",
	"age": `25`
}
````

```json
{
	"firstName": "John",
	"lastName": "Smith",
	"age": 25
}
```

```c
#include <stdio.h>

int main() {
	printf("hello world\n");
	return 0;
}
```

Here's a simple footnote,[^1] and here's a longer one.[^bignote]

[^1]: This is the first footnote.

[^bignote]: Here's one with multiple paragraphs and code.

	Indent paragraphs to include them in the footnote.

	`{ my code }`

	Add as many paragraphs as you like.
