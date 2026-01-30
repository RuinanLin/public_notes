# CHAPTER 1 - The Vim Way

## Tip 1 - Meet the Dot Command

The dot command repeats the last change. A change can be `x`, `dd`, or `>G`.

We can also create a change each time we dip into Insert mode. From the moment we enter Insert mode until we return to Normal mode, Vim records every keystroke. After making a change such as this, the dot command will replay our keystrokes.

We can think of the dot command as being a miniature macro.

## Tip 2 - Don't Repeat Yourself

Instead of using `$a;<Esc>` then `j$.`, using `A` command can reduce extraneous movement.

There are some similar compound commands (only show things that I'm not familiar with):

| **Compound Command** | **Equivalent in Longhand** |
| --- | --- |
| `C` | `c$` |
| `s` | `cl` |
| `S` | `^C` |
| `I` | `^i` |

## Tip 3 - Take One Step Back, Then Three Forward

### `;` and `,` commands

First of all, let's meet the `;` and `,` commands. If you use `f`, `F`, `t`, and `T` commands frequently, then `;` and `,` commands are your best friends.

After you've finished a search within a line, for example, `f{char}`, and you wish to jump to the next `{char}`, you don't need to press `f{char}` again. Instead, `;` will help you jump to the next in the same direction. `,` will help you jump to the opposite direction.

Example snippet:

```javascript
var total = price + tax + shipping;
```

1. After pressing `fp`, the cursor will jump to the `p` character in word `price`;
2. Then press `;`, the cursor will jump to the first `p` in `shipping`.

### Make the Motion and the Change Repeatable

Example snippet:

```javascript
var foo = "method("+argument1+","+argument2+")";
```

We wish to turn it into:

```javascript
var foo = "method(" + argument1 + "," + argument2 + ")";
```

1. `f+`;
2. `s + <Esc>`;
3. `;`;
4. `.`;
5. ...

## Tip 4 - Act, Repeat, Reverse

(Nothing to be recorded.)

## Tip 5 - Find and Replace by Hand

(Nothing to be recorded.)

## Tip 6 - Meet the Dot Formula

Dot Formula: One Keystroke to Move, One Keystroke to Execute.

