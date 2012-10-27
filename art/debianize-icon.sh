#!/bin/bash

uuencode -m icon-m-lookalike.png icon-m-lookalike.png | sed -e s,^,\ ,  > icon-m-lookalike.base64
