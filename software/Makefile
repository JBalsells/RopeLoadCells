VENV_DIR := venv
PYTHON_VERSION := 3.10

all: venv install

venv:
	@echo "Creating virtual environment..."
	python3 -m venv $(VENV_DIR)
	@. venv/bin/activate && \
	@echo "Virtual environment created."
	@echo 

install:
	@echo "Installing packages from requirements.txt..."
	@sudo apt update && sudo apt install -y python3-pip
	@sudo pip install --upgrade pip
	@sudo pip install --no-cache-dir -r requirements.txt

clean:
	@echo "Removing virtual environment..."
	rm -rf $(VENV_DIR)
	@echo "Virtual environment removed."
