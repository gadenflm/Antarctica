# A sample Markdown document

This is a sample document so you can preview the color themes.

## I am a second-level header

Markdown supports _italics_, __bold__, and ___bold italics___ style.

There are also inline styles like `inline code in monospace font` and ~~strikethrough style~~. __There may be ~~strikethroughed text~~ or `code text` inside bold text.__ _And There may be ~~strikethroughed text~~ or `code text` inside italic text._

To reference something from a URL, [Named Links][links] and [Inline links](https://example.com/index.html) are of great help. Sometimes ![A picture][sample image] is worth a thousand words.

There are two types of lists, numbered and unnumbered.

1. Item 1
2. Item 2
3. Item 3

* Item A
    - Sub list
        + Sub sub list
        + Sub sub list 2
    - Sub list 2
* Item B
* Item C

## Fenced code

You can write fenced code inside three backticks.

```javascript
function fibo(n) {
    fibo.mem = fibo.mem || []; // I am some comment
    return fibo.mem[n] || fibo.mem[n] = n <= 1 ? 1 : fibo(n - 1) + fibo(n - 2);
}
```

## The following section is used to define named links

[links]: https://example.com/index.html
[sample image]: https://example.com/sample.png

## Wiki links

This [[SamplePage]] is a wiki link

---

