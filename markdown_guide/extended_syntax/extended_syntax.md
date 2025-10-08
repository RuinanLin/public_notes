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

### My Great Heading {#custom-id}[^2]

[A link to custom ID](#custom-id)

[^2]: As you can see, github does not support custom IDs. IDs are auto-generated only. When you write `## My Section`, it will automatically generate an ID like `my-section`, and we can link to it using `[Go to section](#my-section)`

First Term
: This is the definition of the first item.

Second Term
: This is one definition of the second item.
: This is another definition of the second item.[^3]

[^3]: As you can see, Github Flavored Markdown (GFM) does not support definition lists.

~~The world is flat.~~ We now know that the world is round.

- [x] Write the press release
- [ ] Update the website
- [ ] Contact the media

🫠

Gone camping! :tent: Be back soon.

That is so funny! :joy:

I need to highlight these ==very important words==.[^4]

[^4]: Does not work in github 🙁

I need to highlight these <mark>very important words</mark>.

H~2~O[^4]

H<sub>2</sub>O

X^2^[^4]

X<sup>2</sup>
