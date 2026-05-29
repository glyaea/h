# h

A minimal program for daily tasks on MacOS.

## Installation

Clone repository.

Run command:
```sh
cd h
install h /usr/local/bin
```

## Usage

Run command:
```sh
h
```

Use Up or Down to move, and ENTER to select option:
```
hello! >w<

  [*] clone repo
  [ ] ship repo
  [ ] update h
  [ ] exit h
```

### `clone repo`

Enter input:
```
>w<

  owner/project: 
```

Input `foo/bar` runs command:
```sh
git clone https://github.com/foo/bar
```

Input `foo` runs command:
```sh
git clone "$(curl -s "https://api.github.com/search/repositories?q=foo&per_page=1" \
| jq -r ".items[0].html_url")"
```

### `ship repo`

Runs commands:
```sh
git add .
git commit --allow-empty-message --no-edit
git push
```

### `update h`

Runs commands:
```sh
curl -fsSL "https://raw.githubusercontent.com/gregorylimeurhen/h/refs/heads/main/h" -o ./h
install h /usr/local/bin
```

### `exit h`

Runs command:
```sh
exit
```
