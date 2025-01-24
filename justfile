build:
	scons compiledb=true

update:
    git pull --rebase upstream master
    git submodule -q foreach git pull -q origin master
