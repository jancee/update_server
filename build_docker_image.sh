#!/bin/bash

docker build . -t jancee/update-server:latest
docker tag jancee/update-server:latest seeed.azurecr.io/seeed-update-image-generator:1.0
docker tag jancee/update-server:latest seeed.azurecr.io/seeed-update-image-generator:latest



