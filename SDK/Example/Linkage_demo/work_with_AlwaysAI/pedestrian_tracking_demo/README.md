# Eyecloud-Starter-App

Basic application to get you started working with Eyecloud Cameras.

## Note
Eyecloud containers are in this format: alwaysai/edgeiq:eyecloud-\<arch\>-\<version\>.

Eyecloud models in the model catalogue are denoted with `eyecloud` in their name.

## Setup
This app requires an alwaysAI account. Head to the [Sign up page](https://www.alwaysai.co/dashboard) if you don't have an account yet. Follow the instructions to install the alwaysAI tools on your development machine.
You can refer the url(https://www.alwaysai.co/docs/get_started/development_computer_setup.html) to setu runtime envirment

Next, create an empty project to be used with this app. When you clone this repo, you can run `aai app configure` within the repo directory and your new project will appear in the list.

## Usage
Once you have the alwaysAI tools installed and the new project created, run the following CLI commands at the top level of the repo:

To set the project and select the target device, run:

```
aai app configure
```

To build your app and install on the target device:

```
aai app install
```

To start the app:

```
aai app start
```
