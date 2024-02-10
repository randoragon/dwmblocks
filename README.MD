# dwmblocks

Randoragon's build of dwmblocks, a modular status bar for dwm written in c. See
the original repo at
[github.com/torrinfail/dwmblocks](https://github.com/torrinfail/dwmblocks).

## Modules

Since some modules need a lot of shell trickery to work, it's easier to write
entire dedicated scripts, add them to PATH and have dwmblocks run clean, short
commands. For simplicity's sake I don't maintain those scripts directly in this
repo, instead they can be found in my [dotfiles
repository](https://github.com/Randoragon/dotfiles/tree/master/scripts/.scripts/dwmblocks).

## Installation

Simply run the following command (if necessary, as root):

    make install
