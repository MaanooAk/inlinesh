# inlinesh

Preprocessor of any kind of file, powered by sh.

**inlinesh** was created to generate fast single html files, but it can be used for everything where you need the output a shell commands into a file.

The first use was something like that:

```xml
<!doctype html>
<html>
<head>
	<meta charset="UTF-8">

#echo "<!-- Generated: $(date) -->"

#import headers.html

	<style>
#sed 's/    /\t/' main.css | sass --scss
	</style>

</head>
</body>

#import body.html

	<script>
#import main.js
	</script>

</body>
```

And by the following the final index.html is generated:

```
inlinesh main.html > index.html
```

## Usage

**inlinesh** reads the files provided as arguments (or from stdin if none is provided) and echos all the lines except lines where the first character is a `#`.

The text after the `#` is considered a shell command and is executed with the `system` function (which probably passes it to `sh -c`) and echos its output instead of the command.

The command `import` is specially handled as a recursive call to **inlinesh** (`cat` can be used to just echo the contents of a file).

The command `include` is hadled as `import` but it keeps track of the files that have been included. The second time the same file is given, the command is ignored.

### Options

- `-f` `--force`: do not terminate if a file cannot be found
- `-c CHAR` `--command CHAR`: use the `CHAR` instead of the `#` as command prefix 

## Install

```
git clone https://github.com/MaanooAk/inlinesh.git
cd inlinesh
sudo make install
```

