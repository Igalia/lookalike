Look Alike
==========

What is it?
-----------

*Look Alike* is an application for the batch mode management of the
face recognition un/tagging feature included in the N9 (and N950)
MeeGo devices. It provides the user with contact suggestions for the
faces recognized in the pictures stored in the device which grow more
accurate the more this feature is used. It allows batch mode
operations for tagging the suggested contacts or untagging pictures
with confirmed contacts on the face areas.

Installing:
-----------

The easiet way of installing Look Alike is through its entry in the
[Nokia Store](http://store.ovi.com/content/327099).

Features:
---------

 * "Gallery" grid view of the pictures stored in the device.
 * "Contacts" list view of the pictures set belonging to a certain
   contact.
 * "Suggestions" list view of the pictures suggested to belong to a
   certain contact.
 * "Landscape" grid view of the pictures stored in the device without
   recognized faces.
 * Confirming and tagging faces of a certain contact from a set of
   pictures in batch mode.
 * Removing confirmed faces of a certain contact from a set of
   pictures in batch mode.
 * Most of the functionality of the official Gallery application.

Usage explanation:
------------------

For a brief usage explanation visit Look Alike's
[page](http://igalia.github.com/lookalike/) or this
[video explanation](http://youtu.be/wauAsm-NO2Q).

Known Issues:
-------------

 * Only *PR >= 1.3* is currently supported.
 * When a new picture with a face is added to the device, Look Alike
   doesn't recognize nor suggest any face automatically; the picture
   must be opened in fullscreen first. Only after that, Look Alike
   will suggest contacts for the found faces.

In depth:
---------

Look Alike is built on top of the *gallerycore* and *facerecognition*
privative libraries provided by Nokia on its Harmattan MeeGo phones N9
and N950.

Combining these 2 components with other MeeGo open source components
like *Tracker* and the *MeegoTouch Framework*, Look Alike provides a
similar functionality to Nokia's official in-built Gallery
application.

In addition, Look Alike provides some extended features focused on the
management of the new face recognition feature included since PR1.2.

Look Alike provides 4 different views.

The first view is a grid view of all the pictures stored in the
device, similarly as what it is featured in the official Gallery
application.

The second view is a list of the contacts who have being confirmed in
any of the pictures. This view is similar to the contacts view in the
official Gallery application, but it also has an extra contact called
*Everybody* for all the pictures which have any contact confirmed on
them.

Also, through this second view it is possible to select an action to
delete several confirmed faces from a certain contact in batch mode.

The third view is a list of the contacts who Look Alike suggests there
are pictures in which detected faces could belong to them and have yet
not being confirmed. In addition, we have an extra contact for all the
pictures which have a detected face but Look Alike is unable to
suggest the contact to whom them belong to.

Also, through this third view it is possible to select an action to
confirm several suggested faces for a certain contact in batch mode.

Finally, the forth view displays the subset of pictures in which Look
Alike has been unable to detect any faces. Hence, these should be
*landscape* pictures.