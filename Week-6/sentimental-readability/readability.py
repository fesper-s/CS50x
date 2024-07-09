from cs50 import get_string


def main():
    text = get_string("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    index = 0.0588 * ((letters / words) * 100) - 0.296 * ((sentences / words) * 100) - 15.8

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {round(index)}")


def count_letters(text):
    letters = 0
    for c in text:
        if c.isalpha():
            letters += 1
    return letters


def count_words(text):
    words = 1
    if text[0] == " ":
        return 0

    for c in text:
        if c == " ":
            words += 1

    return words


def count_sentences(text):
    if text[0] == "":
        return 0

    sentences = 0
    for c in text:
        if c == "." or c == "!" or c == "?":
            sentences += 1
    if sentences == 0:
        sentences += 1

    return sentences


main()
