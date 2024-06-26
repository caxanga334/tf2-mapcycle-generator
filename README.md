# Mapcycle Generator

A program for creating a mapcycle file for TF2 game servers from valve's official casual map rotation.

## Usage

The data is extracted from **items_game.txt**. You need to provide a full path to the items_game.txt file.

Example: `./tf2-mapcycle-generator --itemsgame-path "/home/tf2server/serverfiles/tf/scripts/items/items_game.txt"`

The output file will be printed to the terminal.

## Options

The following options are available for configuring the generator.

* `--include-halloween`: Include halloween maps.
* `--include-christmas`: Include christmas maps.
* `--include-competitive-maps`: Include 6v6 competitive maps.
* `--include-unlisted-maps`: Include unlisted maps.
* `--exclude-core`: Exclude maps tagged as 'core' maps.
* `--exclude-alternatives`: Exclude maps tagged as 'alternative' maps.
* `--no-comments`: Do not add comments to the generated mapcycle file.
* `--no-duplicates`: Do not add duplicate maps to the generated mapcycle file.

