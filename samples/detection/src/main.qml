import QtMultimedia 5.5
import Chilitags 1.0
import QtQuick 2.0
import QtQuick.Scene3D 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtWebKit 3.0
import QtWebKit.experimental 1.0


//The name for a tag is tag_nb (do not add 0 to numbers from 1 to 9 !)

/*
* If you want to add a tag and bind a website to that tag then do the following:
* Step 1: Add a new ChilitagsObject element:
*               ChilitagsObject {
                id: tagNB
                name: "tag_NB"

                onVisibilityChanged: {
                    if(tagNB.visible){
                        webview.url = "The url address of your website";
                    }
                }
            }

  Step 2: Add that tag's id to the chiliobjects array of the Chilitags element.


  The following is a list of the tags and their functions:
  tag_2: Display a gray square fitting the tag and browse http://www.google.com
  tag_10: Go back in the browser history
  tag_20: Go forward in the browser history
  tag_28: Browse https://www.yahoo.com/
  tag_32: Browse https://fr.wikipedia.org
  */

Item {

        Camera{
            id:camDevice
            imageCapture.resolution: "640x480" //Android sets the viewfinder resolution as the capture one
            viewfinder.resolution: "640x480"
        }

        Rectangle{

            id: videoOutputRectangle
            x: 640
            y: 480
            width: 160
            height: 120
            VideoOutput{
                id: videoOutput
                source:camDevice
                anchors.fill: parent
                filters:[chilitags]
                Scene3D{
                    anchors.fill: parent
                    focus: true
                    aspects: "input"
                    AnimatedEntity{
                        tagForThisObject: tag2
                    }
                }
            }

            Chilitags{
                id:chilitags
                chiliobjects: [tag2, tag10, tag20, tag28, tag32]
            }

            ChilitagsObject {
                id: tag10
                name: "tag_10"

                onVisibilityChanged: {
                    if(tag10.visible){
                        if(webview.canGoBack){
                            webview.goBack();
                        }
                        webview.experimental.evaluateJavaScript("window.location.href = 'https://fr.wikipedia.org';");
                    }
                }
            }

            ChilitagsObject {
                id: tag20
                name: "tag_20"

                onVisibilityChanged: {
                    if(tag20.visible){
                        if(webview.canGoForward){
                            webview.goForward();
                        }
                    }
                }
            }

            ChilitagsObject {
                id: tag28
                name: "tag_28"

                onVisibilityChanged: {
                    if(tag28.visible){
                        webview.url = "https://www.yahoo.com/";
                    }
                }
            }

            ChilitagsObject {
                id: tag32
                name: "tag_32"

                onVisibilityChanged: {
                    if(tag32.visible){
                        webview.url = "https://fr.wikipedia.org";
                    }
                }
            }

            ChilitagsObject{
                id: tag2
                name: "tag_2"

                onVisibilityChanged: {

                    if(tag2.visible){
                        webBrowser.width = 160;
                        webBrowser.height = 120;
                        webBrowser.x = 800;
                        webBrowser.y = 640;
                        webBrowser.z = 1;

                        videoOutputRectangle.width = 800;
                        videoOutputRectangle.height = 640;
                        videoOutputRectangle.x = 0;
                        videoOutputRectangle.y = 0;
                        videoOutputRectangle.z = 0;
                        //webview.url = "http://www.google.com"
                    } else {

                        webBrowser.width = 800;
                        webBrowser.height = 640;
                        webBrowser.x = 0;
                        webBrowser.y = 0;
                        webBrowser.z = 0;

                        videoOutputRectangle.width = 160;
                        videoOutputRectangle.height = 120;
                        videoOutputRectangle.x = 640;
                        videoOutputRectangle.y = 480;
                        videoOutputRectangle.z = 1;
                    }
                }
            }
        }

        ScrollView {
            id: webBrowser
            width: 800
            height: 640

            z: -1

            WebView {
                    id: webview
                    url: "http://www.google.com"
                    anchors.fill: parent
                    onNavigationRequested: {
                        // detect URL scheme prefix, most likely an external link
                        var schemaRE = /^\w+:/;
                        if (schemaRE.test(request.url)) {
                            request.action = WebView.AcceptRequest;
                        } else {
                            request.action = WebView.IgnoreRequest;
                            // delegate request.url here
                        }
                    }
                }
        }
}


