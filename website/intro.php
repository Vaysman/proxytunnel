 <?php include("_begin.php"); ?>

<h2 class="headline">ProxyTunnel Introduction</h2>

<P>ProxyTunnel is a program that connects stdin and stdout to a server somewhere on the network, through a standard HTTPS proxy. We mostly use it to tunnel SSH sessions through HTTP(S) proxies, allowing us to do many things that wouldn't be possible without ProxyTunnel.</P>

<P>
Proxytunnel can currently do the following:
<UL>
<LI>Create tunnels using HTTP and HTTPS proxies (That understand the HTTP
CONNECT command).
<LI>Work as a back-end driver for an OpenSSH client, and create SSH connections
through HTTP(S) proxies.
<LI>Work as a stand-alone application, listening on a port for connections, and
then tunneling these connections to a specified destination.
</UL>
</P>

<P>If you want to make effective use of ProxyTunnel, the proxy server you are
going to be tunneling through must adhere to some requirements.
<UL>
<LI>Must support HTTP CONNECT command
<LI>Must allow you to connect to destination machine and host, with or without
HTTP proxy authentication
</UL>
Most proxies however only allow connections to a number of pre-defined ports.
These ports usually include 80(http) and 443(https). Some other proxies also
allow traffic on other ports or ranges. Try to find out what ports your proxy
allows you to connect through. Your best guess is to test for 80 and
443, and then check for some other common ports like 8000, 8080, 8081, 8082 and
the nntp ports 119(nntp) and 563(snntp).</P>

<P>If you have figured out what ports your proxy allows you to connect through,
the fun can start. If it allows you to connect to a port you want access too,
like the pop3 or imap ports you are in luck, since you can now set up a direct
tunnel to these servers and read mail for example. Usually however you will be
stuck with access to only port 80 and 443 (like we were, when we wrote
ProxyTunnel). To be able to get access to more then just these ports you need
access to a server on the internet where you are able to log-in via SSH on one
of these ports. In our case we set up the SSH daemons on our home and office
systems to listen to port 443(https), since these weren't used (and 80 was), and
the port was allowed by the firewall/proxy.</P>

<P>After having setup a SSH daemon on an accessable port, we configured our
local SSH clients to use ProxyTunnel as a back-end to make the connection. Doing
this involves creating a ~/.ssh/config file, specifying a host-alias there, and
telling SSH to use a proxy-command, using the ProxyCommand statement and our
ProxyTunnel tool to do it.</P>

<P>We now have access to SSH on our 'unrestricted' system on the internet. As
you may know, SSH allows you to do port-forwarding and other nice tricks. Using
this knowledge it is possible to forward and port anywhere. I myself usually
setup some port-forwardings for my mail (2 imap tunnels) and usenet. But i'm
sure you can think up of many things you'd like to connect to. Now you can.</P>


<?php include("_end.php"); ?>
