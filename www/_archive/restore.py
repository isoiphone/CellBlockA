#!/usr/bin/env python3
"""Rebuild/verify the 1999 and 2001 sites from saved Wayback payloads.

No network, third-party packages, or game execution required.
"""
import argparse
import hashlib
import json
import re
import sys
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urlsplit

ROOT = Path(__file__).resolve().parents[1]
RAW = ROOT / "_archive" / "original"
PAGES = ("index.html", "screens.html", "download.html", "links.html")
COPIES = {
    **{name: name for name in (
        "box2.gif", "fireext.gif", "hand.gif", "hbar.gif", "links.gif",
        "rebar.gif", "screens.gif", "shot1a.gif", "shot2a.gif", "shot2b.gif",
        "shot3a.gif", "shotgun.gif", "spindle1.gif", "spindle2.gif",
        "star1.gif", "star2.gif", "toroid2.gif",
    )},
    "fhs.jpg": "pics/fhs.jpg",
    "shot1b.gif": "pics/shot1b.gif",
    "shot3b.gif": "pics/shot3b.gif",
}
ALT = {
    "box2.gif": "Main", "star1.gif": "Screens (current page)",
    "star2.gif": "Screens", "toroid2.gif": "Download",
    "spindle1.gif": "Links", "spindle2.gif": "Links (current page)",
    "screens.gif": "Screen Shots", "links.gif": "Links",
    "fhs.jpg": "Flaming Head Software", "hand.gif": "Hands",
    "rebar.gif": "Rebar", "fireext.gif": "Fire Extinguisher",
    "shotgun.gif": "Shotgun", "shot1a.gif": "Some guy jumping",
    "shot2a.gif": "Getting sprayed by fire extinguisher",
    "shot3a.gif": "Watch out for that rubble", "hbar.gif": "",
}


def render_1999(name):
    # Netscape's declared Latin-1 was interpreted as Windows-1252 by browsers.
    # Retain these original bytes and the original charset declaration.
    text = (RAW / name).read_bytes().decode("cp1252")
    text = text.replace('<html>', '<html lang="en">', 1)
    text = text.replace('</head>', '''   <!-- Local preservation changes are documented in restoration.html. -->
   <style>
   .restored-heading { display: inline-block; font-family: "Times New Roman", serif;
       font-weight: bold; line-height: 1; text-align: center; }
   .preservation-note { font-size: 12px; text-align: center; margin-top: 32px; }
   </style>
</head>''', 1)
    # These five graphics have no successful capture in the saved CDX index.
    text = text.replace('<img SRC="cellblocka.gif" height=45 width=314>',
        '<span class="restored-heading" style="width:314px;height:45px;font-size:42px" '
        'title="Text substitute: original title graphic was not recovered">Cell Block A</span>')
    text = text.replace('<img SRC="download.gif" height=43 width=259>',
        '<span class="restored-heading" style="width:259px;height:43px;font-size:40px" '
        'title="Text substitute: original heading graphic was not recovered">Download</span>')
    text = text.replace('<img SRC="e-mail.gif" BORDER=0 height=14 width=123>',
        '<span title="Text substitute for the missing email graphic">kevlarx@netidea.com</span>')
    text = text.replace('SRC="box1.gif"', 'SRC="box2.gif" title="Main (current page); surviving alternate icon"')
    text = text.replace('SRC="toroid1.gif"', 'SRC="toroid2.gif" title="Download (current page); surviving alternate icon"')
    text = text.replace('http://www.netidea.com/~kevlarx/cellin13.exe', '../downloads/cellin13.exe')
    # No live CGI requests or fabricated visitor count.
    text = re.sub(r'<img SRC="http://www\.netidea\.com/cgi-bin/Count\.cgi\?[^\"]+"[^>]*>',
        '<!-- Original server-side visitor counter unavailable in this static copy. -->', text)

    def add_alt(match):
        tag = match.group(0)
        source = re.search(r'SRC="([^\"]+)"', tag).group(1)
        return tag[:-1] + ' alt="' + ALT.get(source, source) + '">'

    text = re.sub(r'<img\b[^>]*>', add_alt, text, flags=re.I)
    text = text.replace('</body>', '''<p class="preservation-note">Preserved 1999-era website &middot;
<a href="restoration.html">Restoration notes and sources</a> &middot;
<a href="../index.html">Choose website version</a></p>
</body>''')
    return text.encode("cp1252")


def render_2001(name):
    text = (RAW / "2001" / name).read_bytes().decode("cp1252")
    text = text.replace('<html>', '<html lang="en">', 1)
    text = text.replace('</head>', '''   <!-- Local preservation changes are documented in restoration.html. -->
   <style>
   font[face="OCR A Extended"] { font-family: "OCR A Extended", "OCR A", monospace; }
   font[face="Terminal"] { font-family: Terminal, monospace; }
   .preservation-note { font: 12px serif; text-align: center; margin-top: 32px; }
   </style>
</head>''', 1)
    # Missing background strips: retain the original charcoal table fill and dimensions.
    text = re.sub(r' BACKGROUND="\./pics/topbar-(top|middle|bottom)\.jpg"',
        ' title="Original decorative background not recovered; original table color retained"', text)
    for button in ("download", "links"):
        text = text.replace(f'src="./pics/button-{button}-clicked.jpg"',
            f'src="./pics/button-{button}.jpg" title="Current page; surviving alternate button"')
    # All listed installer mirrors now use the same preserved local installer.
    text = re.sub(r'href="(?:https?|ftp)://[^\"]*/cellin13\.exe"',
        'href="../downloads/cellin13.exe"', text)
    text = text.replace('http://www.netidea.com/~kevlarx/maps01.zip', '../downloads/maps01.zip')
    text = re.sub(r'<a href=http://www\.honesty\.com><img[^>]+></a>',
        '<!-- Original remote visitor counter omitted. -->', text)

    def add_alt(match):
        tag = match.group(0)
        source = re.search(r'src="([^\"]+)"', tag, flags=re.I).group(1)
        filename = source.rsplit('/', 1)[-1]
        if filename.startswith('button-'):
            label = filename.removeprefix('button-').removesuffix('.jpg').replace('-clicked', ' (current page)').capitalize()
        else:
            label = ALT.get(filename, filename)
        return tag[:-1] + ' alt="' + label + '">'

    text = re.sub(r'<img\b[^>]*>', add_alt, text, flags=re.I)
    text = text.replace('</body>', '''<p class="preservation-note">Preserved 2001 website &middot;
<a href="restoration.html">Restoration notes and sources</a> &middot;
<a href="../index.html">Choose website version</a></p>
</body>''')
    return text.encode("cp1252")


class References(HTMLParser):
    def __init__(self):
        super().__init__()
        self.refs = []

    def handle_starttag(self, tag, attrs):
        for key, value in attrs:
            if value and key in ("src", "href", "background"):
                self.refs.append(value)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Verify without modifying files")
    args = parser.parse_args()
    manifest = json.loads((ROOT / "_archive" / "manifest.json").read_text())
    for entry in manifest["files"]:
        path = ROOT / entry["saved_path"]
        if hashlib.sha256(path.read_bytes()).hexdigest() != entry["sha256"]:
            raise ValueError(f"Preserved source hash mismatch: {path}")
    outputs = {f"1999/{name}": render_1999(name) for name in PAGES}
    outputs.update({f"1999/{dest}": (RAW / source).read_bytes() for dest, source in COPIES.items()})
    outputs.update({f"2001/{name}": render_2001(name) for name in PAGES})
    outputs.update({f"2001/pics/{p.name}": p.read_bytes() for p in (RAW / "pics").iterdir() if p.is_file()})
    # These two images survive at the earlier URL, but not in the later pics/ directory.
    outputs.update({f"2001/pics/{name}": (RAW / name).read_bytes() for name in ("shot2a.gif", "shot2b.gif")})
    for name, data in outputs.items():
        path = ROOT / name
        if args.check:
            if not path.exists() or path.read_bytes() != data:
                raise ValueError(f"Restored output differs: {path}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
    checked = 0
    pages = ["index.html"] + [f"{year}/{name}" for year in ("1999", "2001") for name in (*PAGES, "restoration.html")]
    for name in pages:
        parsed = References()
        historical = name != "index.html" and Path(name).name in PAGES
        parsed.feed((ROOT / name).read_bytes().decode("cp1252" if historical else "utf-8"))
        for ref in parsed.refs:
            url = urlsplit(ref)
            if url.scheme or url.netloc or not url.path:
                continue
            path = (ROOT / name).parent.joinpath(unquote(url.path)).resolve()
            if not path.is_relative_to(ROOT) or not path.exists():
                raise ValueError(f"Broken local reference in {name}: {ref}")
            checked += 1
    print(f"Verified {len(manifest['files'])} source hashes, {len(outputs)} restored files, "
          f"and {checked} local references across {len(pages)} pages.")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError) as exc:
        sys.exit(str(exc))
