Contents

Introduction
Target Audience
About OpenSSL
About X.509
Practical OpenSSL Usage
Basic OpenSSL Commands
SSL Certificates
SSL Certificates for Server Use
Using PKCS#12 Certificates in Client Applications
Configuring Apache for SSL Support
Resources
Local System Resources
WWW Resources

Introduction
This guide is designed to introduce the reader to the Secure Sockets Layer (SSL) application-level protocol, and particularly the OpenSSL implementation of SSL. After a brief description of exactly what OpenSSL is, and what it is useful for, the guide will further illustrate the practical usage of OpenSSL in a client-server environment, and provide specific examples of applications which may benefit from OpenSSL. Finally, the guide will lead the user through example procedures required to use OpenSSL with the popular Apache Hyper Text Transport Protocol (HTTP) server for the purpose of serving secured web pages from your Ubuntu computer.


Target Audience
To properly implement the practical steps found in this guide, the reader should be a user of Ubuntu who is comfortable with the use of command-line applications, using the Bourne Again SHell (bash) environment, and editing system configuration files with their preferred text editor application. While previous familiarity with Server Sockets Layer (SSL), or the OpenSSL implementation in particular, is not required for this guide, if desired, the reader is advised to pursue further learning from the resources listed in the Resources section of this guide in order to broaden his/her understanding of this powerful security layer.


About OpenSSL
Secure Sockets Layer is an application-level protocol which was developed by the Netscape Corporation for the purpose of transmitting sensitive information, such as Credit Card details, via the Internet. SSL works by using a private key to encrypt data transferred over the SSL-enabled connection, thus thwarting eavesdropping of the information. The most popular use of SSL is in conjunction with web browsing (using the HTTP protocol), but many network applications can benefit from using SSL. By convention, URLs that require an SSL connection start with https: instead of http:.

OpenSSL is a robust, commercial-grade implementation of SSL tools, and related general purpose library based upon SSLeay, developed by Eric A. Young and Tim J. Hudson. OpenSSL is available as an Open Source equivalent to commercial implementations of SSL via an Apache-style license.


About X.509
X.509 is a specification for digital certificates published by the International Telecommunications Union - Telecommunication (ITU-T). It specifies information and attributes required for the identification of a person or a computer system, and is used for secure management and distribution of digitally signed certificates across secure Internet networks. OpenSSL most commonly uses X.509 certificates.


Practical OpenSSL Usage

Installing OpenSSL Toolkit
To install the OpenSSL binary toolkit, install the following packages openssl (see InstallingSoftware).


Installing OpenSSL Library
To install the OpenSSL general-purpose library, first determine the applicable version of the library available for your Ubuntu computer with the following command issued at a terminal prompt:

apt-cache search libssl | grep SSL

You should observe output from the command similar to the following:


libssl0.9.6 - SSL shared libraries (old version)
libssl-dev - SSL development libraries, header files and documentation
libssl0.9.7 - SSL shared libraries
In the above example, you would most likely want to install the current OpenSSL library, which appears in the output as libssl0.9.7 (like sudo apt-get install libssl0.9.7. Install the following packages libssl0.9.7 (see InstallingSoftware). You may also need to install ca-certificates.


Installing OpenSSL Development Library
In order to build software which requires the OpenSSL general-purpose library, you must first install the development instance of the OpenSSL library. Install the following packages libssl-dev (see InstallingSoftware). Due to OpenSSL's license being incompatible with the GPL, linking the OpenSSL library with programs covered by GPL requires an explicit linking exception for packages present in the Ubuntu Archive. (Ubuntu Technical Board decision)


Basic OpenSSL Commands
The following section of the guide presents some of the more common basic commands, and parameters to commands which are part of the OpenSSL toolkit. For additional information, read the various OpenSSL system manual pages with the man command, and refer to the information presented in the Resources section of this guide.

Determine installed OpenSSL version:

openssl version

List of available OpenSSL sub-commands:

openssl help

Get additional help information on OpenSSL sub-commands by using the openssl command followed by the sub-command, and the -h switch. For example, to get additional information on the openssl enc sub-command:

openssl -h enc

List all available cipher algorithms:

openssl ciphers -v

You may benchmark your computer's speed with OpenSSL, measuring how many bytes per second can be processed for each algorithm, and the times needed for sign/verify cycles by using the following command:

openssl speed


SSL Certificates
The following sections of this guide will introduce the concepts involved in the generation and use of SSL certificates, both the self-signed variety, and those signed by a recognized certificate authority for use with a server application supporting SSL, and the use of X.509 certificates in client applications.


SSL Certificates for Server Use
Once you have properly generated an X.509-compliant SSL certificate, you may either elect to sign the certificate yourself, by generating a Certificate Authority (CA), or you may opt to have a globally recognized Certificate Authority sign the certificate. When the certificate is signed, it is then ready to be used with the OpenSSL toolkit, or the library to enable encrypted SSL connections to a Lightweight Directory Access Protocol, (LDAP) or Hyper Text Transport Protocol (HTTP) server, for example. This section of the guide describes the certificate generation, and signing process for both self-signed, and recognized CA-signed certificates.


Generating and Signing Self-Signed Certificates
Self-signed certificates have a major advantage in that they are completely free to use, and they may be generated, signed, and used on an as-needed basis. Self-signed certificates are great for use in closed-lab environments or for testing purposes. One of the drawbacks of using self-signed certificates, however, is that warnings will typically be issued by a user's Web browser, and other applications, upon accessing an SSL-secured server that uses a self-signed certificate. By default, client applications (e.g., Firefox) will suppress such warnings for certificates that are signed using only a globally-recognized and trusted Certificate Authority, but warnings may also be squelched by importing a server's root certificate into client applications; a relevant demonstration is shown later in this guide. Using self-signed certificates in a publicly-accessible, production environment is not recommended due to the implicit trust issues arising from these warnings, in addition to the potential confusion caused to users.

IconsPage/note.png NOTE: You must obtain a certificate signed by a recognized Certificate Authority in order to establish a commercial site, e.g., for conducting "e-commerce".

Provided you've installed the OpenSSL toolkit previously, or per instructions above, the generation of X.509 SSL certificates is quite simple. For self-signed certificates, you must first establish a Certificate Authority (CA) by following the steps below:


Creating the Certificate Authority
First, create an initial working environment, for example within your home directory by issuing the following command from a terminal prompt:


cd && mkdir -p myCA/signedcerts && mkdir myCA/private && cd myCA
The above command will place you in a newly-created subdirectory of your home directory named myCA, and within this subdirectory, you should have two additional subdirectories named signedcerts and private.

Within this initial working environment, the significance of the subdirectories, and their contents is as follows:

~/myCA : contains CA certificate, certificates database, generated certificates, keys, and requests

~/myCA/signedcerts : contains copies of each signed certificate

~/myCA/private : contains the private key

Next, create an initial certificate database in the ~/myCA subdirectory with the following command at a terminal prompt:

echo '01' > serial  && touch index.txt

Now create an initial caconfig.cnf file suitable for the creation of CA certificates. Using your favorite editor, edit the file ~/myCA/caconfig.cnf, and insert the following content into the file:

sudo nano ~/myCA/caconfig.cnf

# My sample caconfig.cnf file.
#
# Default configuration to use when one is not provided on the command line.
#
[ ca ]
default_ca      = local_ca
#
#
# Default location of directories and files needed to generate certificates.
#
[ local_ca ]
dir             = /home/<username>/myCA
certificate     = $dir/cacert.pem
database        = $dir/index.txt
new_certs_dir   = $dir/signedcerts
private_key     = $dir/private/cakey.pem
serial          = $dir/serial
#       
#
# Default expiration and encryption policies for certificates.
#
default_crl_days        = 365
default_days            = 1825
default_md              = sha1
#       
policy          = local_ca_policy
x509_extensions = local_ca_extensions
#
#
# Copy extensions specified in the certificate request
#
copy_extensions = copy
#       
#
# Default policy to use when generating server certificates.  The following
# fields must be defined in the server certificate.
#
[ local_ca_policy ]
commonName              = supplied
stateOrProvinceName     = supplied
countryName             = supplied
emailAddress            = supplied
organizationName        = supplied
organizationalUnitName  = supplied
#       
#
# x509 extensions to use when generating server certificates.
#
[ local_ca_extensions ]
basicConstraints        = CA:false
#       
#
# The default root certificate generation policy.
#
[ req ]
default_bits    = 2048
default_keyfile = /home/<username>/myCA/private/cakey.pem
default_md      = sha1
#       
prompt                  = no
distinguished_name      = root_ca_distinguished_name
x509_extensions         = root_ca_extensions
#
#
# Root Certificate Authority distinguished name.  Change these fields to match
# your local environment!
#
[ root_ca_distinguished_name ]
commonName              = MyOwn Root Certificate Authority
stateOrProvinceName     = NC
countryName             = US
emailAddress            = root@tradeshowhell.com
organizationName        = Trade Show Hell
organizationalUnitName  = IT Department
#       
[ root_ca_extensions ]
basicConstraints        = CA:true
IconsPage/warning.png IMPORTANT: Make sure to adjust the obvious site-specific details in the file, such as the two instances of /home/<username>/ under [ local_ca ] and [ req ]. Also change commonName, stateOrProvinceName countryName etc under [ root_ca_distinguished_name ] to personalize for your environment. For more information on the directives contained within this configuration file, use the man config command.

When you've edited the file to match your environment, save the file as ~/myCA/caconfig.cnf.

Next, you need to generate the Certificate Authority Root Certificate and Key, by issuing a few commands. First, do this:

export OPENSSL_CONF=~/myCA/caconfig.cnf
The previous command sets an environment variable, OPENSSL_CONF, which forces the openssl tool to look for a configuration file in an alternative location (in this case, ~/myCA/caconfig.cnf).

Now, generate the CA certificate and key with the following command:

openssl req -x509 -newkey rsa:2048 -out cacert.pem -outform PEM -days 1825
You should be prompted for a passphrase, and see output similar to this:


Generating a 2048 bit RSA private key
.................................+++
.................................................................................................+++
writing new private key to '/home/bshumate/myCA/private/cakey.pem'
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
-----
Do not forget the passphrase used with the command above! You'll need it every time you want to generate and sign a new server or client certificate!

The above process will create a self-signed certificate using PEM format and RSA public/private key encryption. The certificate will be valid for 1825 days. The location, and purpose of the resultant files is as follows:

~/myCA/cacert.pem : CA public certificate

~/myCA/private/cakey.pem : CA private key


Optional Step
Strip the certificate from all its text to keep only the -CERTIFICATE- section to create a crt

openssl x509 -in cacert.pem -out cacert.crt

Creating a Self-Signed Server Certificate
Now that you have a Certificate Authority configured, you may use it to sign self-signed certificates. Prior to beginning the steps below, you may wish to encrypt the certificate's private key with a passphrase. The advantages of encrypting the key with a passphrase include protection of the certificate in the event it is stolen.

The certificate cannot be used with SSL-enabled applications without entering the passphrase every time the SSL-enabled application is started. This condition, while being most secure, can present a problem: If the server must be started in an unattended manner as in the case of a computer restart, then no one will be available to enter the passphrase, and subsequently the server will not start. One way to eliminate this condition involves a trade-off in security: The key may be decrypted, to remove the passphrase necessity; thus SSL-enabled applications will start automatically, without a need for you to enter a passphrase.

To actually generate a self-signed certificate for use with an SSL application, follow this process:

Create the server configuration file, by editing ~/myCA/exampleserver.cnf with your favorite text editor. Add this example content:


#
# exampleserver.cnf
#

[ req ]
prompt                  = no
distinguished_name      = server_distinguished_name
req_extensions          = v3_req

[ server_distinguished_name ]
commonName              = tradeshowhell.com
stateOrProvinceName     = NC
countryName             = US
emailAddress            = root@tradeshowhell.com
organizationName        = My Organization Name
organizationalUnitName  = Subunit of My Large Organization

[ v3_req ]
basicConstraints        = CA:FALSE
keyUsage                = nonRepudiation, digitalSignature, keyEncipherment
subjectAltName          = @alt_names

[ alt_names ]
DNS.0                   = tradeshowhell.com
DNS.1                   = alt.tradeshowhell.com
Be sure to change the values under server_distinguished_name especially the commonName value. The commonName value must match the host name, or CNAME for the host you wish to use the key for. If the commonName does not match the intended hostname, then host / certificate mismatch errors will appear in the client applications of clients attempting to access the server.

Once you've edited the file appropriately, save it as ~/myCA/exampleserver.cnf. Generate the server certificate, and key with the following commands:

export OPENSSL_CONF=~/myCA/exampleserver.cnf

The previous command sets an environment variable OPENSSL_CONF which forces the openssl tool to look for a configuration file in an alternative location (in this case, ~/myCA/exampleserver.cnf).

Now generate the certificate, and key:

openssl req -newkey rsa:1024 -keyout tempkey.pem -keyform PEM -out tempreq.pem -outform PEM

You should be prompted for a passphrase, and see output similar to this:


Generating a 1024 bit RSA private key
...++++++
...............++++++
writing new private key to 'tempkey.pem'
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
-----
Don't forget the passphrase!

Next, you may translate the temporary private key into an unencrypted key by using the following command:

openssl rsa < tempkey.pem > server_key.pem

You should be prompted for the passphrase used above, and see the following output:


Enter pass phrase:
writing RSA key
If you wish to leave the key encrypted with a passphrase, simply rename the temporary key using the following command, instead of following the step above:

mv tempkey.pem server_key.pem

IconsPage/tip.png Remember: If you use a server key encrypted with a passphrase, the passphrase will have to be entered each time the server application using the encrypted key is started. This means the server application will not start unless someone, or something enters the key.

Now you need to sign the server certificate with the Certificate Authority (CA) key using these commands:

export OPENSSL_CONF=~/myCA/caconfig.cnf

The previous command modifies the environment variable OPENSSL_CONF which forces the openssl tool to look for a configuration file in an alternative location (in this case, ~/myCA/caconfig.cnf to switch back to the CA configuration).

Then sign the certificate as follows:

openssl ca -in tempreq.pem -out server_crt.pem

You will be prompted for the passphrase of the CA key as created in the Certificate Authority setup from above. Enter this passphrase at the prompt, and you will then be prompted to confirm the information in the exampleserver.cnf, and finally asked to confirm signing the certificate. Output should be similar to this:


Using configuration from /home/bshumate/myCA/caconfig.cnf
Enter pass phrase for /home/bshumate/myCA/private/cakey.pem:
Check that the request matches the signature
Signature ok
The Subject's Distinguished Name is as follows
commonName            :PRINTABLE:'tradeshowhell.com'
stateOrProvinceName   :PRINTABLE:'NC'
countryName           :PRINTABLE:'US'
emailAddress          :IA5STRING:'root@tradeshowhell.com'
organizationName      :PRINTABLE:'Trade Show Hell'
organizationalUnitName:PRINTABLE:'Black Ops'
Certificate is to be certified until Jan  4 21:50:08 2011 GMT (1825 days)
Sign the certificate? [y/n]:y


1 out of 1 certificate requests certified, commit? [y/n]y
Write out database with 1 new entries
Data Base Updated
Remove the temporary certificate, and key files with the following command:

rm -f tempkey.pem && rm -f tempreq.pem

Congratulations! You now have a self-signed server application certificate, and key pair:

server_crt.pem : Server application certificate file

server_key.pem : Server application key file

Use the documentation provided with the server application you wish to use the certificate, and key for in order to properly use it. See the Configuring Apache for SSL Support section below for an example usage.


Optional Step for Certain Server Applications
Some server applications, such as the Courier IMAP mail server application require that the unencrypted private key be prepended to the server certificate. To accomplish this, simply enter the following commands:


cat server_key.pem server_crt.pem > hold.pem
mv      hold.pem server_crt.pem
chmod   400      server_crt.pem

Converting X.509 Certificates to PKCS#12 for Client Applications
If you wish to generate PKCS#12 certificates from your server's Root CA X.509 certificate for client use, you will need to use the following process on the particular server certificate, and key pair you desire to export a client certificate for:

Create a single file containing both the certificate, and key with the following command:

openssl req -x509 -nodes -days 365 -newkey rsa:1024 -keyout mycert.pem -out mycert.pem

Then, convert this certificate / key combination file into the PKCS#12 certificate with the following command:

openssl pkcs12 -export -out mycert.pfx -in mycert.pem -name "Certificate for Whatever"

You will be prompted for an Export password, which you may use, or just leave blank.

The resultant mycert.pfx file may now be imported into applications such as Web Browsers, and E-Mail clients. Since this certificate represents the Root CA certificate of your server, all subsequent server-level certificates signed by the CA certificate will also be automatically accepted by the client application which installs this PKCS#12 version of the certificate.


Generating Certificate for Signing by Recognized CA
In order to run a production server which will engage in commercial activity, such as "e-commerce", it is required to generate a certificate, and have it signed by a recognized Certificate Authority (CA) such as VeriSign. The process for generating a certificate request, and obtaining a certificate signed by a recognized CA can be difficult. You must complete all requisite "paperwork" prior to creating a certificate request, and ensure all required information is accurate.

Assuming you do not wish a passphrase-encrypted key, enter the following command to generate the private key, and certificate request:

openssl req -new -newkey rsa:1024 -nodes -keyout mykey.pem -out myreq.pem

If you already have a key you wish to use, then use the following command instead:

openssl req -new -key mykey.pem -out myreq.pem

You may wish to verify the signature, and information contained in the certificate request. Verify the signature with this command:

openssl req -in myreq.pem -noout -verify -key mykey.pem

and verify the information with this command:

openssl req -in myreq.pem -noout -text

The next steps typically involve pasting the content of the certificate request file into the CA's signup form, and awaiting the certificate. Also, you should safeguard the key file, as it will be required to use the certificate you receive from the CA.


Using PKCS#12 Certificates in Client Applications
This section of the guide explains using PKCS#12 certificates for SSL connections from the client perspective, and demonstrates the process of importing certificates into the Firefox Web Browser applications, and a couple of the more popular E-Mail clients.

Importation of a root certificate into such applications enables a trusted SSL-encrypted connection to the server from whence the certificate came, free of nagging messages about the certificate being self-signed, and so on.


Importing a Certificate into Mozilla Firefox
Importation of a PKCS#12 certificate into the Mozilla Firefox Web Browser application is very simple:

From within Firefox, click Edit > Preferences

Click the Advanced icon

Click the View Certificates button

Click the Import button

Browse to the location of the certificate file, which is typically a .pfx file type

Highlight the certificate file, and click the Open button

You may be prompted to enter Firefox's Master Password at this point, or to set an initial Master Password. Enter the current, or net password as required, confirm it, and click OK

You will then be prompted to enter the certificate's Export password. If there is no such password attached to the certificate, simply click OK, otherwise enter the password, and click OK

A message will appear advising that Firefox has "Successfully restored your security certificate(s) and private key(s)"
Click OK

You have successfully imported the server's client PKCS#12 certificate into Firefox

Importing a Certificate into Evolution
Importation of a PKCS#12 certificate into the Evolution E-Mail client couldn't be simpler:

From within Evolution, click Edit > Preferences

Click the Certificates icon

Click the Import button

Browse to the location of the certificate file, which is typically a .pfx file type

Highlight the certificate file, and click the Open button

You may be prompted to enter Evolution's certificate database password at this point, or to set an initial certificate database password. Enter the current, or new password as required, confirm it, and click OK

You will then be prompted to enter the PKCS12 File Password. If there is no such password attached to the certificate, simply click OK, otherwise enter the password, and click OK

You should see the certificate, and its details appear in the Your Certificates section of the Evolution Settings window

Click the Close button

You have successfully imported the server's client PKCS#12 certificate into Evolution

Importing a Certificate into Mozilla Thunderbird
Importation of a PKCS#12 certificate into the Mozilla Thunderbird E-Mail client application is very simple:

From within Thunderbird, click Edit > Preferences

Click the Advanced icon

Click the Certificates entry in the right pane

Click the Manage Certificates button

Click the Import button

Browse to the location of the certificate file, which is typically a .pfx file type

Highlight the certificate file, and click the Open button

You may be prompted to enter Thunderbird's Master Password at this point, or to set an initial Master Password. Enter the current, or new password as required, confirm it, and click OK

You will then be prompted to enter the certificate's Export password. If there is no such password attached to the certificate, simply click OK, otherwise enter the password, and click OK

A message will appear advising that Thunderbird has "Successfully restored your security certificate(s) and private key(s)"
Click OK

You have successfully imported the server's client PKCS#12 certificate into Thunderbird

Importing a Certificate into the System-Wide Certificate Authority Database
You can import a CA Certificate into the system-wide database of trusted certificate authorities. Applications that use this database will automatically trust any certificates stored here.

1. Copy your certificate to the system certificate directory. At a terminal prompt, type:

$ sudo cp mycert.pem /usr/share/ca-certificates/mycert.crt
2. Edit the ca-certificates configuration file /etc/ca-certificates.conf. Add the name of the file you copied to /usr/share/ca-certificates to the top of the list just after the final "#". For example:

# This file lists certificates that you wish to use or to ignore to be
# installed in /etc/ssl/certs.
# update-ca-certificates(8) will update /etc/ssl/certs by reading this file.
#
# This is autogenerated by dpkg-reconfigure ca-certificates.
# certificates shoule be installed under /usr/share/ca-certificates
# and files with extension '.crt' is recognized as available certs.
#
# line begins with # is comment.
# line begins with ! is certificate filename to be deselected.
#
mycert.crt
brasil.gov.br/brasil.gov.br.crt
cacert.org/cacert.org.crt
mozilla/ABAecom_=sub.__Am._Bankers_Assn.=_Root_CA.crt
[... many additional certificates omitted ...]
Instead of manually editing this file, you can run

sudo dpkg-reconfigure ca-certificates

3. Update the CA certificates database by typing:

$ sudo update-ca-certificates
4. You have successfully imported your certificate into the system CA certificates database.


Configuring Apache for SSL Support
To configure Apache2 for HTTPS and generate a certificate, follow the instructions in Server Guide - HTTPD and Server Guide - Certificates.


Resources
Additional resources pertaining to OpenSSL and Secure Sockets Layer appear below.


Local System Resources
man config

System manual page for the OpenSSL library configuration files

man gendsa

System manual page for the gendsa DSA private key generator

man genrsa

System manual page for the genrsa RSA private key generator

man openssl

System manual page for the openssl command-line tool

man rand

System manual page for the rand pseudo-random byte generator utility

man x509

System manual page for the x509 certificate display and signing utility


WWW Resources
CACert, a FREE X.509 Certificate Authority

OpenSSL Website

Public Key Infrastructure (X.509) (pkix)

CategorySecurity CategorySecurity CategorySecurity
