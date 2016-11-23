time ./morphopen --input "$1" --width $2 --height $3 --output "$4" --levels "$5"
convert -size 2550x3300 -depth 8 gray:"$4" "${4}.png"

