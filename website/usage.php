<?php include("_begin.php"); ?>

<h1 class="headline">Using ProxyTunnel</h1>

<p>The document below is a brief overview of the command-line options supported in proxytunnel. If you are looking for a more in-depth article about the features of proxytunnel, and what concequences using it brings along, please read <A HREF="paper.php">Muppet's paper</A>.

<div class="rightside">
 <h3>Notice</h3>
 <h4>Document needs work</h4>
 <p>This page is/was somewhat out of date, It's currently partially updated to reflect changes in the current version of proxytunnel. Therefor some parts of this document may be incorrect, reflecting proxytunnel how it was back in the 1.1 days. Many of the new features are not (yet) documented here, hopefully this will be fixed soon ;)</p>
 <p class="info"><HR>Added 2006/02/10 by Maniac</p>
</div>

<P>Proxytunnel is very easy to use, when running proxytunnel with the help
option it specifies it's commandline options.</P>

<PRE>
# proxytunnel --help
Proxytunnel 1.6.0
Copyright 2001-2006 Proxytunnel Project
Jos Visser (Muppet) <josv@osp.nl>, Mark Janssen (Maniac) <maniac@maniac.nl>

Purpose:
  Build generic tunnels trough HTTPS proxy's, supports HTTP authorization

Usage: Proxytunnel [OPTIONS]...
   -h         --help              Print help and exit
   -V         --version           Print version and exit
   -i         --inetd             Run from inetd (default=off)
   -a INT     --standalone=INT    Run as standalone daemon on specified port
   -e         --encrypt           encrypt the communication using SSL
   -p STRING  --proxy=STRING      Proxy host:port combination to connect to
   -d STRING  --dest=STRING       Destination host:port to built the tunnel to

Parameters for proxy-authentication (not needed for plain proxies):
   -u STRING  --user=STRING       Username to send to HTTPS proxy for auth
   -s STRING  --pass=STRING       Password to send to HTTPS proxy for auth
   -U STRING  --uservar=STRING    Env var with Username for HTTPS proxy auth
   -S STRING  --passvar=STRING    Env var with Password for HTTPS proxy auth
   -N         --ntlm              Use NTLM Based Authentication
   -t STRING  --domain=STRING     NTLM Domain (default: autodetect)
   -r STRING  --remproxy=STRING   Use a remote proxy to tunnel over (2 proxies)
   -H STRING  --header=STRING     Add STRING to HTTP headers sent to proxy

  If you don't provide -s or -S you will be prompted for a password.

Miscellaneous options:
   -v         --verbose           Turn on verbosity (default=off)
   -q         --quiet             Suppress messages  (default=off)

Examples:
Proxytunnel [ -h | -V ]
Proxytunnel -i [ -u user ] -p proxy:port -d host:port [ -v | -q ]
Proxytunnel -i [ -U envvar ] -p proxy:port -d host:port [ -v | -q ]
Proxytunnel -a port -p proxy:port -d host:port [ -v | -q ]
</PRE>

To use this program with OpenSSH to connect to a host somewhere, create
a $HOME/.ssh/config file with the following content:

<PRE>
	Host foobar
		ProtocolKeepAlives 30
		ProxyCommand /path/to/proxytunnel -p proxy.customer.com:8080 -u user -s password -d mybox.athome.nl:443
</PRE>

With:

<TABLE BORDER=1>

	<TR><TD>foobar</TD><TD>The symbolic name of the host you want to connect to</TD></TR>
	<TR><TD>proxy.customer.com</TD><TD>The host name of the proxy you want to connect through</TD></TR>
	<TR><TD>8080</TD><TD>The port number where the proxy software listens to</TD></TR>
	<TR><TD>user</TD><TD>Your proxy userid</TD></TR>
	<TR><TD>password</TD><TD>Your proxy password</TD></TR>

	<TR><TD>mybox.athome.nl</TD><TD>The hostname of the box you want to connect to (ultimately)</TD></TR>
	<TR><TD>443</TD><TD>The port number of the SSH daemon on mybox.athome.nl</TD></TR>
</TABLE></P>

<P>If your proxy doesn't require the username and password for using it,
you can skip these options. If you only specify the username, you will be asked for a password at connection-time<P>

<P>The ProtocolKeepAlives line is used to send occasional NULL packets (if your SSH version supports this). This is used to keep the connection up, since HTTPS proxy's will usually drop the connection if it remains idle for some time. Experiment with the time between the NULL packets (in seconds)</P>

<P>Most HTTPS proxies do not allow access to ports other than 443 (HTTPS)
and 563 (SNEWS), so some hacking is necessary to start the SSH daemon on
the required port. (On the server side add an extra Port statement in
the sshd_config file)</P>

<P>When all this is in place, execute an "ssh foobar" and you're in business!</P>

<P>Share and Enjoy!</P>
<?php include("_end.php"); ?>
