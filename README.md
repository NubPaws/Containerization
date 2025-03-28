# Docker Containerization

This repository's intention is to teach the coder how to containerize their application using Docker. The repository is split into different `md` files for docker installation, project layout, and the actual containerization.

The goal is for us to have a simple project (that can be extended and has enough features to cover a lot of use cases) that can be run by any user, on any machine, using the following commands:
```bash
git clone https://github.com/Username/RepositoryName
cd RepositoryName
docker-compose up --build
```

If you are interested, you can look at the [Why should you containerize?](#why-should-you-containerize) if not, we will present here the terminology. You may go ahead and skip to your relevant part in this repository using the following menu:

This tutorial will be aimed at beginners who want/need to containerize their project.

## Menu
1. [How to setup docker](./guides/setup.md)
   1. [Windows](./guides/setup.md#windows)
   2. [Windows using WSL](./guides/setup.md#windows-using-wsl)
   3. [MacOS](./guides/setup.md#macos)
   4. [Linux](./guides/setup.md#linux)
2. [Project layout explanation]()
3. [Containerizing]()

## Terminology
- **Docker**
  <br/>
  The tool we use to manage our containers, images, volumes, and networks.
- **Docker Hub**
  <br/>
  The official registry/repository of Docker images, from which Docker downloads images, and to which we can upload or images to having them be version controlled.
- **Image**
  <br/>
  A template from which we create containers. Images are readonly, meaning they cannot change. A change to an image is a new version.
- **Container**
  <br/>
  A local instance of an image, running our code. Unlike images, containers can (and will) be changed.
  <br/>
  A container is similar to a virtual machine, however much lighter as the container and the host share the same kernel (a virtual machine has it's own kernel emulation).
- **Docker Compose**
  <br/>
  A tool for defining and running multi-container applications. If you have only one container to run, you can just use `docker build` rather than `docker-compose` and it will be simpler.

*Note.* This guide will not go into the `docker build` command as we plan on setting up a multi-container environment. That shouldn't scare us though.

## Links to further resources
You can find a plethora of information at the [Docker docs](https://docs.docker.com/) that contains a lot of helpful information split into the following sections:
- [Docker guides](https://docs.docker.com/guides/)
- [Docker manuals](https://docs.docker.com/manuals/)
- [Docker references](https://docs.docker.com/reference/)


## Why should you containerize?
There are a lot of reasons, but the main one is that you are saving the person viewing your repository the hassle of configuring your project to run on their machine, and the hassle of following a complex setup. This begs the question.

Why shouldn't you just have a script then, one for Linux, one for Mac, and one for Windows, setting up the environment and your project and running it?

The answer is simple. A lot of people, including myself, will not run a script they don't know what it does fully. In addition, a lot of people, including myself, don't feel like reading 100s of lines of scripting code to make sure that there's no malicious code hidden there.

A container prevents this as everything you are running is in a container, which is essentially just a virtual machine that has restrictions placed on it.

Another good reason to containerize is that it adds another technology to your project stack, which is a nice bonus. In addition, Docker (containers in general) are used all throughout the industry for running things in production mode.

