# Setup Docker and Docker Compose

- [Setup Docker and Docker Compose](#setup-docker-and-docker-compose)
	- [Windows](#windows)
	- [Windows using WSL](#windows-using-wsl)
	- [MacOS](#macos)
	- [Linux](#linux)

## Windows

1. Go to [docker.com](https://docker.com).
2. Hover over "Download Docker Desktop" and click on "Download for Windows" matching your AMD or ARM processor.
3. Install Docker, making sure to use WSL (Windows Subsystem for Linux when prompted).
4. During the installation when prompted:
   1. "Enable Hyper-V wWindows Features" (if using Windows 10/11 Pro).
   2. "Enable WSL 2 Windows Features" (for WSL 2).
5. Verify that docker compose is installed (it should be installed by default when installing Docker Desktop) by running the following in a command prompt:
   ```bash
   docker compose version
   ```
   or
   ```bash
   docker-compose version
   ```

**Important.** If you are prompted about using Linux or Windows containers, choose Linux containers for simplicity, unless you have a specific reason to use Windows containers (which for this tutorial and for the most part you probably won't have).

## Windows using WSL

Follow the instructions provided in the [windows](#windows) installation guide above. After that you want to enable intergration with your WSL distribution.
1. In Docker Desktop, go to **Settings → Resources → WSL Integration**
2. Toggle on the distribution(s) where you want Docker to be enabled.
3. Test Docker and Docker Compose in WSL by running the following:
   ```bash
   docker run hello-world
   docker compose version
   ```
   or
   ```bash
   docker run hello-world
   docker-compose version
   ```

## MacOS
1. Go to [Install Docker Desktop on Mac](https://docs.docker.com/desktop/setup/install/mac-install/).
2. Download the version matching your time of machine (Apple silicon or Intel chip).
3. Install by dragging the Docker.app icon to your `Applications` folder.
4. Lunch docker desktop by double clicking the `Docker.app`.
5. Once docker is up and running verify that you have docker compose installed by running the following in the terminal:
   ```bash
   docker compose version
   ```
   or
   ```bash
   docker-compose version
   ```

## Linux
For linux we will use the `install-docker.sh` script. It is highly recommended that you go to https://get.docker.com and follow the instructions provided there as they are the most up to date.

1. Download the `install-docker.sh` script:
   ```bash
   curl -fsSL https://get.docker.com -o install-docker.sh
   ```
2. Verify the script's content:
   ```bash
   cat install-docker.sh
   ```
3. Run the script with --dry-run to verify the steps it executes:
   ```bash
   sh install-docker.sh --dry-run
   ```
4. Run the script either as root, or as sudo to perform the installation:
   ```bash
   sudo sh install-docker.sh
   ```
5. (Optional but recommended) Add our user to the Docker group so that we don't have to run it using `sudo` every time:
   ```bash
   sudo usermod -aG docker $USER
   ```
6. Verify the installation of docker and docker compose running:
   ```bash
   docker version
   docker compose version
   ```
   or
   ```bash
   docker version
   docker-compose version
   ```
