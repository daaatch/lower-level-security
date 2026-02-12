X509v3 Authority Key Identifier: 
                C6:6F:3B:85:7B:D1:26:B1:78:9A:42:A4:25:69:0C:F6:FF:7A:A0:67
            Authority Information Access: 
                CA Issuers - URI:http://cacert.pec.it/certs/arubapec-eidas-g1
                OCSP - URI:http://ocsp01.pec.it/va/arubapec-eidas-g1
                
$ openssl ocsp -issuer issuer_certs.pem -cert extracted_cert.pem -url http://ocsp01.pec.it/va/arubapec-eidas-g1

Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            60:7e:88:fc:e0:f3:c5:00:7c:11:11:2d:90:20:46:a5
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: C = IT, L = Arezzo, O = ArubaPEC S.p.A., organizationIdentifier = VATIT-01879020517, OU = Qualified Trust Service Provider, CN = ArubaPEC EU Qualified Certificates CA G1
        Validity
            Not Before: Nov  5 13:24:18 2024 GMT
            Not After : Nov  5 13:24:18 2027 GMT
        Subject: C = IT, SN = d'Amore, GN = Fabrizio, serialNumber = TINIT-DMRFRZ60P04H501I, CN = Fabrizio d'Amore, dnQualifier = WSREF-94746602542849
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (2048 bit)
                Modulus:
                    00:b9:1b:83:9d:a4:44:66:43:9f:a7:c5:5c:30:97:
                ...
                    04:cb:ad:f2:12:8c:ba:f0:67:73:96:a2:c1:3f:2d:
                    aa:6b
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Authority Key Identifier: 
                C6:6F:3B:85:7B:D1:26:B1:78:9A:42:A4:25:69:0C:F6:FF:7A:A0:67
            Authority Information Access: 
                CA Issuers - URI:http://cacert.pec.it/certs/arubapec-eidas-g1
                OCSP - URI:http://ocsp01.pec.it/va/arubapec-eidas-g1
            X509v3 Issuer Alternative Name: 
                email:info@arubapec.it
            X509v3 Certificate Policies: 
                Policy: 0.4.0.194112.1.2
                Policy: 1.3.6.1.4.1.29741.1.7.2
                  CPS: https://www.pec.it/repository/arubapec-qualif-cps.pdf
                Policy: 1.3.76.16.6
            qcStatements: 
                0..0......F..0......F.....0......F..0.......F..0..0>.8https://www.pec.it/repository/arubapec-qualif-pds-it.pdf..it0>.8https://www.pec.it/repository/arubapec-qualif-pds-en.pdf..en
            X509v3 CRL Distribution Points: 
                Full Name:
                  URI:http://crl01.pec.it/va/arubapec-eidas-g1/crl
            X509v3 Subject Key Identifier: 
                25:60:64:8E:3E:5D:07:11:36:E3:91:34:6C:02:1B:95:CA:E5:8E:5F
            X509v3 Key Usage: critical
                Non Repudiation
    Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        71:b8:42:c0:34:3e:41:7e:89:64:ef:7d:a7:fe:c8:fe:9b:9d:
       ...
        b1:39:89:27:eb:16:07:7e:b6:d0:87:14:ae:78:ff:d7:a3:23:
        14:21:7c:8a:85:6b:8f:54

