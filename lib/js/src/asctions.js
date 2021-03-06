// Generated by BUCKLESCRIPT VERSION 1.7.4, PLEASE EDIT WITH CARE
'use strict';

var $$Array                 = require("bs-platform/lib/js/array.js");
var Js_json                 = require("bs-platform/lib/js/js_json.js");
var Caml_array              = require("bs-platform/lib/js/caml_array.js");
var Js$slashhelpers         = require("js/helpers");
var Js_primitive            = require("bs-platform/lib/js/js_primitive.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function unwrapUnsafely(param) {
  if (param) {
    return param[0];
  } else {
    throw [
          Caml_builtin_exceptions.invalid_argument,
          "unwrapUnsafely called on None"
        ];
  }
}

var Option = /* module */[/* unwrapUnsafely */unwrapUnsafely];

function getPics() {
  return fetch("http://mapd-cats.azurewebsites.net/catpics").then(function (prim) {
                return prim.text();
              }).then(function (xml) {
              return Promise.resolve(Js$slashhelpers.gatherImages(xml));
            });
}

function getFacts() {
  return fetch("http://mapd-cats.azurewebsites.net/catfacts").then(function (prim) {
                return prim.json();
              }).then(function (json) {
              var d = unwrapUnsafely(Js_json.decodeObject(json));
              return Promise.resolve(unwrapUnsafely(Js_json.decodeArray(unwrapUnsafely(Js_primitive.undefined_to_opt(d["facts"])))));
            });
}

function getCats() {
  return Promise.all(/* array */[
                getPics(/* () */0),
                getFacts(/* () */0)
              ]).then(function (param) {
              if (param.length !== 2) {
                throw [
                      Caml_builtin_exceptions.match_failure,
                      [
                        "/Users/Tai/workspace/cats-re/src/asctions.re",
                        30,
                        10
                      ]
                    ];
              } else {
                var images = param[0];
                var facts = param[1];
                return Promise.resolve($$Array.to_list($$Array.mapi(function (i, x) {
                                    var img = unwrapUnsafely(Js_json.decodeString(x));
                                    var fct = unwrapUnsafely(Js_json.decodeString(Caml_array.caml_array_get(facts, i)));
                                    return /* record */[
                                            /* id */i,
                                            /* fact */fct,
                                            /* image */img,
                                            /* isHidden : false */0
                                          ];
                                  }, images)));
              }
            });
}

exports.Option   = Option;
exports.getPics  = getPics;
exports.getFacts = getFacts;
exports.getCats  = getCats;
/* js/helpers Not a pure module */
