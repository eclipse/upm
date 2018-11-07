# Author: Mircea Bardac <mircea.bardac@intel.com>

# default
.PHONY: default
default:
	@echo "Type \"make setup\" to download locally dependencies of DoxyPort"
	@echo "in a Python virtual environment."
	@echo

.PHONY: clean
clean:
	rm -rf env

.PHONY: check
check:
	@which virtualenv > /dev/null || (echo "Setup requires \"virtualenv\" to be installed (pip install virtualenv)" && exit 0)

.PHONY: setup
setup: check
	virtualenv env
	bash -c "source env/bin/activate; pip install -r requirements.txt"

.PHONY: devenv
devenv: check
	virtualenv env
	bash -c "source env/bin/activate; pip install -r devenv.txt"

.PHONY: lint
lint: pylint pycodestyle pydocstyle

.PHONY: pylint
pylint:
	bash -c "source env/bin/activate; pylint doxyport.py --rcfile=.pylintrc"

.PHONY: pycodestyle
pycodestyle:
	bash -c "source env/bin/activate; pycodestyle doxyport.py"

.PHONY: pydocstyle
pydocstyle:
	bash -c "source env/bin/activate; pydocstyle doxyport.py"

.PHONY: test
test:
	bash -c "source env/bin/activate; ./doxyport --help"
