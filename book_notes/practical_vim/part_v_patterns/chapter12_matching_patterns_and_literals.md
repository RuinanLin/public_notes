# CHAPTER 12 - Matching Patterns and Literals

## Tip 72 - Tune the Case Sensitivity of Search Patterns

### `\C`

Since that we've turned both `ignorecase` and `smartcase` on in our `~/.vimrc`, in this part, we will only focus on what we should do under this configuration.

Everything is good except when we wish to search an all-lowercase pattern in a case-sensitive way. In this case, we should append `\C` to force case sensitivity. For example, `/\Cchapter` or `/chapter\C` will only match `chapter` but not `CHAPTER` or `ChApTeR`.

## Tip 73 - Use the \v Pattern Switch for Regex Searches

Example snippet:

```css
body	{ color: #3c3c3c; }
a		{ color: #0000EE; }
strong	{ color: #000; }
```

### Find Hex Colors with Magic Search
    
In Magic Search, we have to use multiple backslashes because most characters are themselves by default and you have to escape it to use it with a special meaning, and also the rule is extremely complicated.
    
```
/#\([0-9a-fA-F]\{6}\|[0-9a-fA-F]\{3}\)
```
    
### Find Hex Colors with Very Magic Search
    
Use the `\v` pattern switch. This enables *very magic* search, where all characters assume a special meaning, with the exception of `_`, uppercase and lowercase letters, and the digits `0` through `9`. If a character has no special meaning, for example, `#`, then it is matched literally.

```
/\v#([0-9a-fA-F]{6}|[0-9a-fA-F]{3})
```
    
### Use the Hex Character Class to Further Refine the Pattern
    
We can replace `[0-9a-fA-F]` with the character class `\x`.

```
/\v#(\x{6}|\x{3})
```
    
## Tip 74 - Use the \V Literal Switch for Verbatim Searches

```
/\Va.k.a.
```

It just searches literally. Use of `\V` means that in the pattern after it, only the backslash has a special meaning.

In conclusion, if you want to search for a regular expression, use the `\v` pattern switch; if you want to search for verbatim text, use the `\V` literal switch.

## Tip 75 - Use Parentheses to Capture Submatches

Example snippet:

```
I love Paris in the
the springtime.
```

We wish to spot the grammatical error: a line break separating two occurences of the same word. Here is a regular expression that matches duplicated words:

```
/\v<(\w+)\_s+\1>
```

If our pattern contains more than one set of parentheses, then we could reference the submatch for each pair of `()` by using `\1`, `\2`, and so on, up to `\9`. The `\0` item always refers to the entire match, whether or not parentheses were used in the pattern. This feature is especially useful in combination with the substitute command, but it can also be used to define patterns where a word is repeated.

Sometimes we may want to use parentheses for grouping and have no interest in capturing the submatch. Then we can prepend `%`, which tells Vim not to bother assigning it to the `\1` register and gets a smidge faster. Example:

```
/\v(%(And|D)rew) (Neil)
:%s//\2, \1/g
```

## Tip 76 - Stake the Boundaries of a Word

Example snippet:

```
the problem with these new recruits is that
they don't keep their boots clean.
```

Instead of typing `/the`, we need `/\v<the>` to match exactly the word `the`.

`<` and `>` are *zero-width* items, meaning that they don't match any characters themselves. We could approximate the `<` item as `\W\zs\w`, and the `>` item as `\w\ze\W`, where `\w` matches word characters, including letters, numbers, and the `_` symbol, while `\W` matches everything except for word characters. In summary, `<` specifies where a word starts, while `>` specifies where a word ends.

Don't forget to use *very magic* search. Otherwise, we have to escape `<` and `>`.

When you use `*` or `#` commands, the search pattern is wrapped with word delimiters automatically. `g*` and `g#` perform without word delimiters.

## Tip 77 - Stake the Boundaries of a Match

Pry pattern and match apart into two separate concepts:

- *pattern*: the regular expression (or literal text) that we type into the search field;
- *match*: any text in the document that appears highlighted (assuming the `hlsearch` option is enabled).

We can use `\zs` and `\ze` to specify a broad pattern and then focus on a subset of the match.

Example 1: Tell the differences among the following three.

```
/\VPractical Vim
/\VPractical \zsVim
/\VVim
```

Example 2: Tell the difference. Notice that `"[^"]+"` is a useful common regex idiom.

```
/\v"[^"]+"
/\v"\zs[^"]+\ze"
```

## Tip 78 - Escape Problem Characters

There are still a few characters whose special meanings can't be turned off by `\V`.

Example snippet:

```markdown
Search items: [http://vimdoc.net/search?q=/\\][s]
...
[s]: http://vimdoc.net/search?q=/\\
```

### Escape / Characters When Searching Forward

If we do not escape it, Vim will interpret the first `/` as a search field terminator.

Why does Vim need to treat `/` as a terminator? The behavior of Vim's search can be tuned by appending certain flags after the search field terminator. For example, `/vim/e` will make the cursor placed at the *end* of the match.

### Escape ? Characters When Searching Backward

It is a similar case. If we search backward, we don't have to escape `/` characters, but instead we have to escape the `?` symbol. Otherwise, it will be treated as a search field terminator.

### Escape \ Characters Every Time

So our final search pattern should be:

```
/\Vhttp:\/\/vimdoc.net\/search?q=\/\\\\
?\Vhttp://vimdoc.net/search\?q=/\\\\
```

### Escape Characters Programmatically

This part involves things about registers. Let's skip it for now and go back later after we've learned about registers.

