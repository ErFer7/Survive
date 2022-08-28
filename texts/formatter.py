# -*- coding: utf-8 -*-

'''
Formata o texto.
'''

texts = ['']

with open("texts.txt", 'r', encoding="utf-8") as file:

    i = 0

    for line in file.readlines():

        if not line.startswith('#'):
            texts[i] += line
        else:
            i += 1
            texts.append('')

formatted_texts = []

for i, text in enumerate(texts):

    formatted_texts.append('')
    formatted_text = ''
    codepoint_line_count = 0

    for c in text:

        ce = c.encode("cp437")
        codepoint = "\\x" + f"{ord(ce):02x}".upper()

        if codepoint_line_count < 19:

            if codepoint_line_count == 0:
                formatted_text += "\"" + codepoint
            else:
                formatted_text += codepoint

            codepoint_line_count += 1
        else:

            formatted_text += codepoint + "\"\n"
            codepoint_line_count = 0

    formatted_text += "\""

    formatted_texts[i] = formatted_text

output = "\n\n".join(formatted_texts)

with open("formatted_texts.txt", 'w+', encoding="utf-8") as file:
    file.write(output)
