# Server VM configuration
$ sudo apt-get update
$ sudo apt-get install safeguard-vpn

$ openssl genpkey -algorithm RSA -out server.key
$ openssl req -new -key server.key -out server.csr
$ openssl x509 -req -in server.csr -signkey server.key -out server.crt

$ nano /etc/safeguard-vpn/server.conf
  server_ip = 192.168.1.1
  vpn_network = 10.8.0.0/24
  vpn_subnet_mask = 255.255.255.0
  vpn_port = 1194
  vpn_protocol = UDP
  server_certificate = /etc/safeguard-vpn/server.crt
  server_key = /etc/safeguard-vpn/server.key
  dh_param = /etc/safeguard-vpn/dh2048.pem
  
$ sudo systemctl start safeguard-vpn-server


