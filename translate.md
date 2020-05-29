
# Guide to create a version of JASP in your own language.
## Summary

<br>Since JASP version 0.12 it is possible to make JASP available for different languages.
The following sections describe the different steps to take to create a JASP international version available for a specific language, and the different file types that are involved. The translation of JASP strongly depends on the creation of '.po' files.
PO (portable object) files are text-based (UTF-8) files, initially generated from JASP prepared source files, which later can be used to extend with translated text for a specific language.
Most of the sections described, are of minor importance for translators who wants to add new  translation of JASP (see section 5). They just serve as a complete list of - mostly developers tasks - to setup an internationalization environment for JASP.
<br>

## Contents:

* [1. Introduction](#1-Introduction)
* [2. Preparing different types of source files](#2-Preparing-different-type-of-source-files)
* [3. Definitions and terms](#3-Definitions-and-terms)
* [4. Preliminaries](#4-Preliminaries)
* [5. Tasks for a translator of JASP](#5-Tasks-for-a-translator-of-JASP)
* [6. References](#5-References)

### 1. Introduction.

In JASP version 0.12 all JASP source files are prepared to make it possible that the JASP software can become internationalized.
In general JASP is divided into two big functional parts. First JASP Interface, containing all the screens to define the different parameter windows and file menu's. JASP-Interface is mainly written in C++ and QML. Secondly JASP-Engine(s), executing all the R-code from different analyses, this part is mainly written in R (and C++ but this code does not produce any screen messages but produces only information for logging purposes).  
In fact three big steps must be taken to make a specific translation of JASP:

1. Prepare all the different type of source file of JASP. This is done in JASP version 0.12 for the first time. All the different type source files involved is described in the [Preparing different type of source files](#2-Preparing-different-type-of-source-files) section.
2. Generate .po files from the different source files. Once all the source files are prepared for translation all the .po file can be generated from the different type of sources. How all the .po files are structured and how they are created out of the source files is described in the 'How to generate .po files' section.
3. Generate binaries that are loaded by JASP at runtime which make it possible to switch between different languages.



### 2. Preparing different type of source files.
This section describes how JASP code must be prepared, so that the - later described - translation tools can generate the appropriate .po files which in turn behave like source files for the actual translation. All the actions described have already been carried out in the latest JASP 0.12 version. But if some translation is still missing, then first the source code must be changed by looking up the untranslated string in the code, and adjust the code as described below. Of course this also applies for new added code.


1. Qml Files

	* a. All literal strings e.g. in a label or text field should be embedded in the
   qsTr function: e.g. _qsTr_ ("Log to file ")
	* b. Use %x to Insert parameters into a string.
   Different languages put words together in different orders so it is not a
   good idea to create sentences by concatenating words and data.
   Instead, use % to insert parameters into strings. For example, the
   following snippet has a string with two number parameters %1 and %2.
   These parameters are inserted with the .arg() function.
   Text{ text: qsTr(“File %1 of %2).arg(counter).arg(total) }
   %1 refers to the first parameter and %2 refers to the second parameter
   so this code produces output like: "File 2 of 3".

2. C++ files

	*  a. In the .cpp files you should the tr() function for all literal text
       e.g. errorMsg = tr("Refreshing the analysis may change the results.");
	*  b. Similar as mentioned above use the % character for parameters in a
       string
       for example : errorMsg = tr("Cannot find a source %1 for VariableList
       %2").arg(dropKey).arg(listView->name())

3. R-files

	*  a. All literal strings e.g. in titles or messages must be embedded in the
       gettext() function.
       e.g. title=gettext("Hypothesis")
	*  b. Unicode character may not appear in the literals strings as output
       strings:
       e.g. title="McDonald's \u03C9" must become gettextf(“McDonald's
       %s”,” \u03C9") instead of title=gettext("McDonald's \u03C9")
	    The same is true for a single % character in a gettext. It must be transformed
	    to a gettextf with a double %%. Please report if you know a better solution.
	*  c. All paste and paste0 functions must be replaced by the gettext or the
       gettextf functions. For example:
       overtitle = paste0(100 * options$confidenceIntervalInterval, "% CI for
       Proportion")) becomes :
       overtitle = gettextf("%i%% CI for Proportion", 100 *
       options$confidenceIntervalInterval))
       But it is sometimes also possible to use a combination:
       Paste0(gettext(“This is ok”), gettext(“Not very useful but possible”))
	*  d. Covert sprintf() into gettextf() directly.
	*  e. But further, immediately after % may come 1$ to 99$ to refer to a
       numbered argument: this allows arguments to be referenced out of
       order and is mainly intended for translators of error messages. If this is
       done it is best if all formats are numbered: if not the unnumbered ones
       process the arguments in order. See the example. This notation allows
       arguments to be used more than once, in which case they must be
       used as the same type (integer, double or character).
       E.g.:      
       message <- sprintf("Some entries of %s were not understood. These
       are now grouped under '%s'.", options[["colorNodesBy"]], undefGroup)
       Becomes:
       message <- gettextf("Some entries of %1$s were not understood.
       These are now grouped under '%2$s'.", options[["colorNodesBy"]],undefGroup)
	*  f. Be careful with expressions. Must be considered one by one:
       E.g.
       xName<- expression(paste("Population proportion", ~theta))
       Becomes:
       xName <- bquote(paste(.(gettext("Population proportion")), ~theta))



### 3. Definitions and terms.

* .po files:<br>A .po or Portable Object file is a is text-based editable file. These types of files are commonly  used in software development. The .po file may be referenced by Java programs, GNU gettext, or other software programs as a properties file. These files are saved in a human-readable format so that they can be viewed in a text editor by engineers and translators. <br>The main entries in a .po file are 'msgid' and 'msgstr'. The first entry contains the untranslated message, the second the translated string; e.g. for a Chinese translation file:<br><br>msgid "Free:"<br>
msgstr "免費"<br>

* .pot files: A .pot file or Portable Object Template file, is the file that you get when you extract texts from the application. Normally, this is the base unstranslated file send to translators. It serves as a base for the translation of other language specific .po files. PO and POT files are essentially the same. The difference is in the intended use. This is why the two files have different extensions.
* .mo files:<br>MO or Machine Object is a binary data file that contains object data referenced by a program. It is typically used to translate program code, and may be loaded or imported into the GNU gettext program. In our case these files are generated at the R-side part by means of the GNU gettext tools.
* .qm files: qm or Qt Multi language file is a compiled Qt translation file. In many ways it is similar to gettext, in that it uses a hashing table to lookup the translated text. In older version they store only the hash and the translation which doesn't make the format useful for recovering translated text.

### 4. Preliminaries.

##### Qt linguistic tools from Qt 5.14.2 installation.

Qt provides two specific command line tools for its internationalization.<br>
- lupdate:The lupdate command line tool finds the translatable strings in the specified source, header and Qt Designer interface files, and produces or updates .po translation files.<br>
- lrelease : The lrelease command line tool produces QM files out of .po files. The QM file format is a compact binary format that is used by the localized application. It provides extremely fast lookups for translations.<br>
These two tools come with the installation Qt. <br>Download the Open Source version from (https://www.qt.io/download). It's easy to install but also described in https://github.com/jasp-stats/jasp-desktop/blob/stable/Docs/development/jasp-building-guide.md

##### R 3.6.1
For generation the .mo files R is required. JASP 0.12 is using version 3.6.1 but any later version will do also. The installation is quite straight forward after the download from https://cran.r-project.org/

##### GNU gettext tool set
This toolset offers to programmers, translators and even users, a well integrated set of tools and documentation. Specifically, the GNU gettext utilities are a set of tools that provides a framework within which other free packages may produce multi-lingual messages. This tool set is used under the hood by generating the R-side .mo files.

Installations on MacOS: <br>
\> brew install gettext<br>\> brew link --force gettext<br><br>
On Windows:<br>
Download Windows binaries from: http://gnuwin32.sourceforge.net/packages/gettext.htm<br>
This delivers a gettext-0.14.4-bin.zip. This file has to be unzipped and the location added to the PATH environment.


##### Preparation of structure the R-side (especially for the JASP and JASPgraphs package).<br>
This subject is intended for developers, adding new translatable R-packages to JASP,  
Each R package/dynamic module needs the following folder structure,

* DESCRIPTION
* NAMESPACE
* nst
* po*
* R
* po**

po* and po** can be automatically generated by tools::update_pkg_po("~/path/to/root/of/package"). This function requires the following tools from the GNU: xgettext, msgmerge, msgfmt, msginit and msgconv (see also https://developer.r-project.org/Translations30.html).
The installation of the GNU tool set is needed to get setup this structure.

Note that R does not set the meta data of the .pot and .po files correctly.
You might have to do this yourself. The R package poio which can do some of the meta data corrections for you. It might be more user-friendly than using the functions from tools described below.

To initiate a translation for a module:

2.	Initialize the po folders with tools::update_pkg_po("~/path/to/root/of/package"); this also creates a package .pot file in po\*\*; this file is used later to update the individual translation files.
3.	Get an empty translation template by running tools::xgettext2pot("~/path/to/root/of/package", "~/path/to/root/of/package/R-<language>.po")
4.	Translate the strings in the language specific .po file.
5.	Run tools::update_pkg_po("~/path/to/root/of/package"), this ensures all existing .po files in po** are updated from the .pot file and then compiled and installed in po*.
6.	Ensure that the locale of R is set correctly when the R processes start (see https://cran.r-project.org/doc/manuals/r-patched/R-admin.html#Localization-of-messages). Or specify this during run time throughSys.setenv(LANG = "<language>")



### 5. Tasks for a translator of JASP
As described above, by far the most user interfaces, messages and errors etc. shown by JASP are contained in (at the moment 3) .po files.  Helpfiles and other text that exists in e.g. json files is not covered at the moment, but is planned in the next release of JASP.

#### *Location of the .po files*
Translaters familiar with github:<br> could clone or fork the jasp-desktop repostory and will find then<br>
..../jasp-desktop/JASP-Desktop/po/jasp.po (and jasp\_nl.po for the Dutch version) Copy this jasp.po to jasp\_xx.po and adjust the header.<br>
..../jasp-desktop/JJASP-Engine/JASP/po/R-JASP.pot. Copy this R-JASP.pot to R-xx.po and adjust the header.<br>
..../jasp-desktop/JASP-Engine/JASPgraphs/po/R-JASPgraphs.pot.  Copy this R-JASPgraphs.pot to R-xx.po and adjust the header.

Or others could use a prepared zip file with jasp-desktop structure:<br>
This compressed zip can be downloaded from:  https://static.jasp-stats.org/PO-Files-JASP-0.12.1-S.zip<br>
Unzipping this file gives you a jasp-desktop folder with only the necessary files for the translation and generating the binary translation files, if you want to test them in you own installation of JASP.<br>

Or could use just the po files zip:<br>
This zip file only contains the .po files that must be translated.<br>
Can be downloaded from: https://static.jasp-stats.org/PO-Files-JASP-0.12.1.zip<br>


#### *Translate the .po files*
In the next section the 'xx' stands for the unique two-letter code of the language.
See https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes for the complete list.

1. Translate the JASP .po interface file: JASP\_xx.po <br>Translate means here: Fill in the empty 'msgstr' id. Every different language has it's own specific .po file. This means that every specific language file contains a two-letter code that defines the (ISO) language name JASP-xx.po e.g.:
	* JASP_nl.po for Dutch translations.
	* JASP_de.po for German translations.

	The first version of this file is in fact a generated template file, without any translated message string. All the message string entries starting with 'msgstr' should be filled in with the proper translated string.

2. Translate the JASP R side .po file: 	R-xx.po (located in jasp-desktop/JASP-Engine/JASP/po)
3. Translate the JASPgraphs .po file:	R-xx.po (located in jasp-desktop/JASP-Engine/JASPgraphs/po)

#### *Create the binary .mo and .qm files*
For translators that might have a complete build environment of JASP: edit JASP-Desktop.pro and the  GENERATE_LANGUAGE_FILES = true. Rebuilding the JASP-Desktop will give you all the binary tranlation files.<br><br>
Tranlators that do want to test their translations on an already installed version of JASP:<br>Be sure thate the first three items of the preliminaries are installed.
The run the following commands from a terminal in the jasp-desktop folder.

1.	The Qt JASP interface .qm file: jasp\_xx.qm<br>From a terminal run: <br> > ~/Qt/5.14.2/clang_64/bin/lrelease ./JASP-Desktop/po/jasp\_xx.po -qm jasp\_xx.qm<br>
This creates a jasp\_xx.qm.<br>

2.	The JASP R related .mo file: R-JASP.mo<br>Start R from a terminal run: <br> > tools::update_pkg_po(paste0(getwd(),"/JASP-Engine/JASP"))<br>
This creates a R-JASP.mo in ./JASP-Engine/JASP/inst/po/xx/LC_MESSAGES

3. The JASPgraphs R related .mo file: R-JASPgraphs.mo<br>Start R from a terminal run: <br> > tools::update_pkg_po(paste0(getwd(),"/JASP-Engine/JASPgraphs"))<br>
This creates a R-JASPgraphs.mo in ./JASP-Engine/JASPgraphs/inst/po/nl/LC_MESSAGES

#### *Test binary translation files by copying them to an already installed JASP version*
Once the binaries are successfully created it is possible to test them in an already installed version of JASP. On startup JASP search for all the typical language coded names, like JASP_nl.qm, and then tries to load that file with that specific language code. If one of those is found with a recognized language code its adds it to the language preference menu, so that on runtime it can be chosen.

Copy the files to the runtime environment:<br><br>
On  MacOS in a standard installation:<br>
Location of jasp\_xx.qm files: /Applications/JASP.app/Contents/Resources/Translations  
Location of .R-JASP.mo /Applications/JASP.app/Contents/MacOS/R/library/JASP/po/xx/LC\_MESSAGES<br>
Location of .R-JASPgraphs.mo /Applications/JASP.app/Contents/MacOS/R/library/JASPgraphs/po/xx/LC\_MESSAGES<br>


On  Windows in a standard installation:<br>
Location of jasp\_xx.qm: C:\Program Files\JASP\resources\Translations  
Location of R-JASP.mo C:\Program Files\JASP\R\library\JASP\R\xx\LC\_MESSAGES<br>
Location of R-JASPgraphs.mo C:\Program Files\JASP\R\library\JASPgraphs\R\xx\LC\_MESSAGES

#### *Delivery*
Translators that are familiar with  Github:<br> Can make a Pull Request containing their translated .po file. If this pull request is merged, the new language should appear in the Preferences->Advanced->Prefereed Language dropdown and could be checked from a nightly build.

Or just make a new JASP issue in the https://github.com/jasp-stats/jasp-issues/issues repository. That new Feature Request Issue should contain the translated .po files with some reference or zip with the translated files. This issue will then be handled by the JASP developers.

#### *Questions*

If there are things unclear or not working, please make an issue in the https://github.com/jasp-stats/jasp-issues/issues repository. Then all the developers are able to answer you.
Or send a direct mail to f.a.m.meerhoff@uva.nl

### 6. References

Internationalization with Qt : https://doc.qt.io/qt-5/qtquick-internationalization.html<br>
Writing source code for translation : https://doc.qt.io/qt-5/i18n-source-translation.html<br>
GNU gettext utilities : https://www.gnu.org/software/gettext/manual/gettext.html<br>
Language codes : https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
