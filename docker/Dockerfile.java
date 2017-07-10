FROM upm-base

# Update apt-get
RUN apt-get -y update && \
    # Java Build Dependencies
    apt-get -y --no-install-recommends install libmraa-java \
    default-jre default-jdk

# Configure Java Home
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64/

# Set Workdir
WORKDIR $UPM_SRC_DIR

CMD bash
