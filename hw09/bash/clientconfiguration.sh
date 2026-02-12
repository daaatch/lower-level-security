# Client VM configuration
$ sudo apt-get update
$ sudo apt-get install safeguard-vpn

$ openssl genpkey -algorithm RSA -out client.key
$ openssl req -new -key client.key -out client.csr
$ openssl x509 -req -in client.csr -signkey client.key -out client.crt

$ nano /etc/safeguard-vpn/client.conf
  server_ip = 192.168.1.1
  vpn_network = 10.8.0.0/24
  vpn_port = 1194
  vpn_protocol = UDP
  client_certificate = /etc/safeguard-vpn/client.crt
  client_key = /etc/safeguard-vpn/client.key
  dh_param = /etc/safeguard-vpn/dh2048.pem
  
$ sudo systemctl start safeguard-vpn-client
