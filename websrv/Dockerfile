FROM debian:12-slim
RUN apt update && \
	apt -y install nginx php8.2 php8.2-fpm php8.2-mysql php8.2-curl

COPY env_dbpwd.conf /etc/php/8.2/fpm/pool.d/
COPY leds.conf /etc/nginx/sites-enabled/default

CMD nginx; php-fpm8.2 -FR
