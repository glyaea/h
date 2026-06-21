# h

## Boarding

```sh
# Install h
install h /usr/local/bin

# Uninstall h
rm /usr/local/bin/h
```

## Usage

Run command:
```sh
h
```

Use <kbd>Up</kbd>, <kbd>Down</kbd>, or <kbd>Enter</kbd> to select option:
```
[*] alphabetize string
[ ] clean cache
[ ] clone repo
[ ] find repo
[ ] rebase branch
[ ] ship repo
```

**`alphabetize string`**

Enter input:
```
string: 
```

Input `foo` runs command:
```sh
printf %s foo | grep -o . | sort | paste -sd ""
```

**`clean cache`**

Runs commands:
```sh
brew cleanup --prune=all
conda clean --all
npm cache clean --force
uv cache clean
```

**`clone repo`**

Enter input:
```
owner/project: 
```

Input `foo/bar` runs command:
```sh
git clone https://github.com/foo/bar
```

**`find repo`**

Enter input:
```
query: 
```

Input `foo` runs command:
```sh
curl -s "https://api.github.com/search/repositories?q=foo&per_page=1" \
| jq -r ".items[0].html_url"
```

**`rebase branch`**

Runs commands:
```sh
git pull --rebase
git push
```

**`ship repo`**

Runs commands:
```sh
git add .
git commit --allow-empty-message --no-edit
git push
```
