#!/usr/bin/env python3
import atexit
import signal
import subprocess
import sys
import termios
import tty


class Utility:
	UP = "\033[A"
	DOWN = "\033[B"
	ENTER = "\r"

	@staticmethod
	def clear_screen():
		print("\033[H\033[J", end="")

	@staticmethod
	def get_input(prompt):
		return input(f"{prompt} ")

	@staticmethod
	def hide_cursor():
		print("\033[?25l", end="")

	@staticmethod
	def restore_terminal(settings):
		Utility.show_cursor()
		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

	@staticmethod
	def show_cursor():
		print("\033[?25h", end="")


def clean_disk():
	subprocess.run(["brew", "cleanup", "--prune=all"])
	subprocess.run(["conda", "clean", "--all"])
	subprocess.run(["npm", "cache", "clean", "--force"])
	subprocess.run(["uv", "cache", "clean"])


def clone_repo():
	while True:
		repository = get_input("owner/project:")
		if repository:
			break
	subprocess.run(["git", "clone", f"https://github.com/{repository}"])


def find_repo():
	while True:
		query = get_input("query:")
		if query:
			break
	response = subprocess.run(
		["curl", "-s", f"https://api.github.com/search/repositories?q={query}&per_page=1"],
		capture_output=True,
		text=True
	)
	subprocess.run(["jq", "-r", ".items[0].html_url"], input=response.stdout, text=True)


def lex_order_string():
	while True:
		string = get_input("string:")
		if string:
			break
	print("".join(sorted(string)))


def silent_ship_repo():
	subprocess.run(["git", "add", "."])
	subprocess.run(["git", "commit", "--allow-empty-message", "--no-edit"])
	subprocess.run(["git", "push"])


if __name__ == "__main__":
	options = [
		("clean disk", clean_disk),
		("clone repo", clone_repo),
		("find repo", find_repo),
		("lex-order string", lex_order_string),
		("silent-ship repo", silent_ship_repo)
	]
	selected = 0
	settings = termios.tcgetattr(sys.stdin)
	atexit.register(Utility.restore_terminal, settings)
	signal.signal(signal.SIGHUP, lambda signum, frame: sys.exit())
	signal.signal(signal.SIGTERM, lambda signum, frame: sys.exit())
	tty.setraw(sys.stdin)
	Utility.hide_cursor()
	while True:
		Utility.clear_screen()
		for index, option in enumerate(options):
			bullet = "*" if index == selected else " "
			print(f"[{bullet}] {option[0]}\r")
		keypress = sys.stdin.read(1)
		if keypress == "\033":
			keypress += sys.stdin.read(2)
		if keypress == Utility.UP:
			selected = (selected + len(options) - 1) % len(options)
		if keypress == Utility.DOWN:
			selected = (selected + 1) % len(options)
		if keypress == Utility.ENTER:
			Utility.restore_terminal(settings)
			Utility.clear_screen()
			options[selected][1]()
			sys.exit()
