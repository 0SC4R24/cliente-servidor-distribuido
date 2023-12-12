FROM ubuntu:latest
LABEL authors="adriantoral;oscarviudez"
LABEL description="Dockerfile para la creación de una imagen de Docker con los binarios de los servicios broker, multmatrix y filemanager."

WORKDIR /practicas

# Abrir los puertos necesarios
EXPOSE 10000
EXPOSE 10001
EXPOSE 10002

# Copiar los binarios de broker
COPY ./out/broker broker

# Copiar los binarios de multmatrix
COPY ./out/mm_server mm_server
COPY ./out/mm_client mm_client

# Copiar los binarios de filemanager
COPY ./out/fm_server fm_server
COPY ./out/fm_client fm_client

# Darle permisos de ejecución a los binarios
RUN chmod +x broker
RUN chmod +x mm_server
RUN chmod +x mm_client
RUN chmod +x fm_server
RUN chmod +x fm_client

# Crear la carpeta de filemanager
RUN mkdir dirprueba

# Copiar el loader
COPY ./loader.sh /loader.sh

ENTRYPOINT ["sh", "/loader.sh"]