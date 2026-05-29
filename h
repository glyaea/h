#!/bin/sh

down=$(printf "\033[B")
enter=$(printf "\r")
escape=$(printf "\033")
up=$(printf "\033[A")

selected=0
settings=$(stty -g)

num_options=4

newline() { printf "\r\n"; }
newscreen() { printf "\033[H\033[J"; }

hidecursor() { printf "\033[?25l"; }
showcursor() { printf "\033[?25h"; }

cleanup() {
	showcursor
	stty "$settings"
	newline
}

next_option() { selected=$(( (selected + 1) % num_options )); }
previous_option() { selected=$(( (selected + num_options - 1) % num_options )); }
option() {
	case "$1" in
		0) printf "clone repo" ;;
		1) printf "ship repo" ;;
		2) printf "update h" ;;
		3) printf "exit h" ;;
	esac
}

trap cleanup EXIT
trap "exit" HUP TERM
stty raw -echo
hidecursor

while :
do
	# draw menu
	newscreen
	printf "hey! >w<"
	newline
	newline
	index=0
	while [ "$index" -lt "$num_options" ]
	do
		bullet=" "
		[ "$index" -eq "$selected" ] && bullet="*"
		printf "  [%s] %s" "$bullet" "$(option "$index")"
		newline
		index=$((index + 1))
	done

	# read keypress
	keypress=$(dd bs=1 count=1 2>/dev/null)
	[ "$keypress" = "$escape" ] && keypress="$keypress$(dd bs=1 count=2 2>/dev/null)"

	# act on keypress
	case "$keypress" in
		"$up") previous_option ;;
		"$down") next_option ;;
		"$enter")
			cleanup
			case "$selected" in
				0)
					while :
					do
						newscreen
						printf ">w<"
						newline
						newline
						printf "  owner/project: "
						IFS= read -r repository
						[ -n "$repository" ] && break
					done
					case "$repository" in
						*/*) git clone "https://github.com/$repository" ;;
						*)
							api=https://api.github.com/search/repositories
							git clone "$(
								curl -s "$api?q=$repository&per_page=1" |
								jq -r ".items[0].html_url"
							)"
							;;
					esac
					;;
				1)
					git add .
					git commit --allow-empty-message --no-edit
					git push
					;;
				2)
					curl -fsSL "https://raw.githubusercontent.com/gregorylimeurhen/h/refs/heads/main/h" -o ./h
					install h /usr/local/bin
					;;
				3)
					printf "cya! >w<"
					newline
					exit
					;;
			esac
			exit
			;;
	esac
done
