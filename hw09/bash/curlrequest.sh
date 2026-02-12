# VPN off
$ curl -o /dev/null -w "%{time_total}\n" "https://speed.cloudflare.com/__down?bytes=500000000"
  % Total       % Received % Xferd  Average Speed   Time    Time     Time  Current
                                     Dload  Upload   Total   Spent    Left  Speed
  100  500000000 0  5000    0     0  34728      0 --:--:-- --:--:-- --:--:-- 35211
  1.43973

# VPN on
$ curl -o /dev/null -w "%{time_total}\n" "https://speed.cloudflare.com/__down?bytes=500000000"
  % Total       % Received % Xferd  Average Speed   Time    Time     Time  Current
                                     Dload  Upload   Total   Spent    Left  Speed
  100  500000000 0  5000    0     0  34728      0 --:--:-- --:--:-- --:--:-- 34790
  2.71462
