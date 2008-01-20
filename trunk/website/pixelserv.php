<HTML XMLNS="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<HEAD>
	<TITLE>Pixelserv Homepage</TITLE>
	<META HTTP-EQUIV="content-type" content="text/html; charset=iso-8859-15" />
	<META HTTP-EQUIV="content-script-type" content="text/javascript" />
	<META HTTP-EQUIV="content-style-type" content="text/css" />
	<META CONTENT="Pixelserv" name="description" />
	<META CONTENT="proxytunnel, proxy, firewall, secure shell, ssh" name="keywords" />
	<META CONTENT="Mark Janssen" name="author" />

	<LINK REL="stylesheet" type="text/css" media="screen,projection" href="style.css" />
	<LINK REL="icon" href="favicon.png" type="image/png" />
</HEAD>

<BODY CLASS="linkindex">

<div class="top">
  <div class="box">
    <H1>Pixelserv</H1>
    <div>Serving the web, 1 pixel at a time</div>
  </div>

  <div class="spacer1">
    <div class="linkbar">
      <A HREF="http://proxytunnel.sourceforge.net/">Proxytunnel home</A> |
      <A HREF="http://proxytunnel.maniac.nl/">Proxytunnel home (maniac.nl)</A> |
      <A HREF="http://sourceforge.net/projects/proxytunnel/">Project Page @ SF.Net</A> |
      <A HREF="http://www.maniac.nl">Maniac.nl</A>
    </div>
  </div>
</div>

<div class="main">
	<H1 class="headline">Pixelserv 1.1</H1>

	Pixelserv is a super minimal webserver, it's one and only purpose
	is serving a 1x1 pixel transparent gif file. Using some creative
	firewalling (netfilter/iptables) rules you can redirect some
	webrequests (for adds for example) to pixelserv.

<H2 class="headline">Authors</H2>
<p>Piet Wintjens originally wrote pixelserv, I've (Mark Janssen) made some
slight modifications and enhancements to it. It's really trivial, but very
handy. :)</p>

<p>The main webpage for pixelserv is
<A HREF="http://proxytunnel.sf.net/pixelserv.html">on SF.net</A></p>

<H2 CLASS="headline">License</H2>
<p>
Pixelserv is covered by the new BSD (no advertising clause) license.</p>

<H2 CLASS="headline">Usage</H2>
<p>
Running pixelserv is very easy. There are currently two versions of pixelserv. One runs as a daemon, the other version runs from inetd. Decide which version suits you better (daemon if its accessed often, inetd if less often or you want easier configurability.)</p>

<H2 CLASS="headline">Daemon version (1.0)</H2>
<P>To run the daemon version of pixelserv, change the 'Port' value to match the tcp-port you want pixelserv to listen on. Then just start it in the background, or have it started from /etc/inittab</P>

<H2 CLASS="headline">Inetd version (1.1)</H2>
<P>The inetd version doesn't have any configuration in pixelserv itself. You can run it on one or more ports easily. Add the following entry (using any portnumber you want) to your '/etc/services' file:<BR>
<PRE>
pixelserv      65000/tcp                       # Pixelserv, add blocker
</PRE>
Then add the pixelserv entry to your '/etc/inetd.conf'
<PRE>
pixelserv stream tcp nowait nobody /path/to/pixelserv.pl pixelserv.pl
</PRE></p>
<H2 CLASS="headline">Download</H2>
<p><UL>
<LI><A HREF="files/pixelserv-inetd.pl.txt">pixelserv-inetd.pl</A> (version 1.1)
<LI><A HREF="files/pixelserv.pl.txt">pixelserv.pl</A> (original version 1.0)
</UL></p>

<H2 CLASS="headline">Links</H2>
<p><UL>
	<LI>Maniac's website: <A HREF="http://www.maniac.nl">www.maniac.nl</A>
</UL></P>

<P>Mark Janssen &lt;<A HREF="mailto:maniac@maniac.nl">maniac@maniac.nl</A>&gt;</P>

<A href="http://sourceforge.net"><IMG src="http://sourceforge.net/sflogo.php?group_id=39840" width="88" height="31" border="0" alt="SourceForge Logo"></A>

<?php include("_end.php"); ?>
