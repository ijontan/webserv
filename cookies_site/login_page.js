
function showPassword() {
    var x = document.getElementById("password");
    if (x.type === "password") {
    x.type = "text";
    } else {
    x.type = "password";
    }
}

/*
creating cookies:
    document.cookie = "username=John Doe; expires=Thu, 18 Dec 2013 12:00:00 UTC; path=/";

    - by default the cookie is deleted when the browser is closed
    - the path parameter, tells the browser what path the cookie belongs to
    - by default the cookie belongs to the current page

reading cookies:
    let x = document.cookie;

changing cookies:
    - same way as how you write it, it just overwrites the old one

deleting a cookie:
    document.cookie = "username=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";

    - you don't have to specify a cookie value when you a delete a cookie
    - just set the expires parameter to a past date

*/

/*
cname : cookie name
cvalue : cookie value
exdays : number of days until the cookies should expire
*/
// function setCookie(cname, cvalue, exdays) {
//     const d = new Date();
//     d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
//     let expires = "expires=" + d.toUTCString();
//     document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
// }

// function getCookie(cname) {
//     let name = cname + "=";
//     let ca = document.cookie.split(';');
//     for(let i = 0; i < ca.length; i++) {
//       let c = ca[i];
//       while (c.charAt(0) == ' ') {
//         c = c.substring(1);
//       }
//       if (c.indexOf(name) == 0) {
//         return c.substring(name.length, c.length);
//       }
//     }
//     return "";
// }

// function checkCookie() {
//     let user = getCookie("username");
//     if (user != "") {
//       alert("Welcome again " + user);
//     } else {
//       user = prompt("Please enter your name:", "");
//       if (user != "" && user != null) {
//         setCookie("username", user, 365);
//       }
//     }
//   }
