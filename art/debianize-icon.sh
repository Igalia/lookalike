#!/bin/bash

uuencode -m icon-l-lookalike.png icon-l-lookalike.png | sed -e s,^,\ ,  > icon-l-lookalike.base64
