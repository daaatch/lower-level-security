X509v3 CRL Distribution Points: 
                Full Name:
                  URI:http://crl01.pec.it/va/arubapec-eidas-g1/crl
            X509v3 Subject Key Identifier: 
                25:60:64:8E:3E:5D:07:11:36:E3:91:34:6C:02:1B:95:CA:E5:8E:5F
            X509v3 Key Usage: critical
                Non Repudiation
                
$ curl -O http://crl01.pec.it/va/arubapec-eidas-g1/crl
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 2006k    0 2006k    0     0  2267k      0 --:--:-- --:--:-- --:--:-- 2267k

$ openssl crl -in crl -text -noout

...
Serial Number: 5DBA8D117B364B69
        Revocation Date: Oct 13 07:07:18 2022 GMT
        CRL entry extensions:
            X509v3 CRL Reason Code: 
                Superseded
    Serial Number: 64D3DD154EB21B741F6C3A7BE0F1424D
        Revocation Date: Aug  7 08:29:33 2025 GMT
    Serial Number: 37C00EAA262BBADF8178D47748D1FB56
        Revocation Date: Jul 24 12:49:07 2025 GMT
    Serial Number: 6C40E26245EAEEA7FACB1A4C6AC0C004
        Revocation Date: May 26 10:37:33 2025 GMT
        CRL entry extensions:
            X509v3 CRL Reason Code: 
                Superseded
    Serial Number: 3BB8F0E7E57B91D8304B13DE4EE9EAC4
        Revocation Date: Dec  3 10:56:19 2024 GMT
        CRL entry extensions:
            X509v3 CRL Reason Code: 
                Superseded
    Serial Number: 2499383D636A00D2A51A50B125C337DC
        Revocation Date: May  9 14:24:21 2025 GMT
        CRL entry extensions:
            X509v3 CRL Reason Code: 
                Superseded
...
    Serial Number: 4DD94A6F626FAB56
        Revocation Date: Mar 31 16:40:18 2023 GMT
        CRL entry extensions:
            X509v3 CRL Reason Code: 
                Superseded
    Serial Number: 48E0041F8854C2BDB07D34FB43319595
        Revocation Date: Jun 15 14:37:03 2025 GMT
    Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        1b:13:49:89:bb:30:a7:73:47:31:47:e8:c8:77:89:fa:8f:11:
        ed:28:da:f4:6e:5e:2b:de:a5:d6:04:32:fc:9e:3f:f9:44:10:
        27:62:e3:46:44:cf:6c:b4:b3:cd:dc:01:25:22:65:c7:2d:9c:
        bc:02:cd:10:88:29:ec:20:a3:16:6b:7b:74:5c:e1:4d:96:99:
        91:9c:28:21:0a:8c:d1:f4:fd:5e:b2:64:99:36:ae:8f:d0:cc:
        fb:f9:17:ba:5c:fe:dc:43:9c:82:cc:d6:5a:19:1d:b1:f0:39:
        c8:ce:5d:c5:03:98:b0:6a:52:89:d1:9d:46:54:00:19:5e:e1:
        26:97:46:d4:d3:8a:90:09:51:a3:f0:00:1f:d0:33:33:c5:09:
        6a:c9:ba:36:8a:25:6e:1b

