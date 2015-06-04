/*
 * Author: Heidi Pan <heidi.pan@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


// extract the field from the class description text
function getField(classDescription, field) {
  var pattern = new RegExp('\\+ __' + field + ':__ [A-Za-z0-9]*');
  var label = new RegExp('\\+ __' + field + ':__');
  if (classDescription) {
    var matched = classDescription.match(pattern);
    if (matched) {
      return (matched[0].replace(label, '')).trim();
    }
  }
  return 'other';
}


// generate html to group modules by the given field (e.g. category/connection type) of its children classes
function listByGroup(modules, classes, field, projectRoot) {

  var moduleClasses = {};
  var modulesByGroup = {};
  var i, j;
  for (i = 0; i < modules.length; i++) {
    moduleClasses[modules[i].name] = [];
  }
  for (i = 0; i < classes.length; i++) {
    moduleClasses[classes[i].module].push(i);
  }
  for (var module in moduleClasses) {
    var classIndices = moduleClasses[module];
    var groups = [];
    for (i = 0; i < classIndices.length; i++) {
      groups.push(getField(classes[classIndices[i]].description, field)); 
    }
    if (groups.length != 0) {
      var group = groups[0];
      var sanitychecked = true;
      for (i = 1; i < groups.length; i++) {
        if (groups[i] != group) {
          sanitychecked = false;
          break;
        }
      }
      if (!sanitychecked) {
        // TODO
      }
      if (group in modulesByGroup) {
        modulesByGroup[group].push(module);
      } else {
        modulesByGroup[group] = [module];
      }
    }
  }
  var groups = Object.keys(modulesByGroup).sort();
  var html = '';
  var pfx = projectRoot + 'modules/';
  for (i = 0; i < groups.length; i++) {
    var group = groups[i];
    html += '<div class="upmGroup"><div class="right-arrow"></div>' + group + '</div><span class="upmModules" style="display:none">';
    var moduleNames = modulesByGroup[group];
    for (j = 0; j < moduleNames.length; j++) {
      var moduleName = moduleNames[j];
      html += '<a href="' + pfx + moduleName + '.html">' + moduleName + '</a>';
    }
    html += '</span>';
  }
  return html;
}
  

// click handler to change direction of arrow and toggle visibility of grouped modules
var onClickHandler = "Y.on('domready', function() {       \
  Y.on('click', function(e) {                             \
    var classes = this.next('.upmModules').toggleView();  \
    if (classes.getStyle('display') == 'none') {          \
      this.one('> div').removeClass('down-arrow').addClass('right-arrow'); \
    } else {                                              \
      this.one('> div').removeClass('right-arrow').addClass('down-arrow'); \
    }                                                     \
  }, '.upmGroup');                                        \
});";


// css to generate triangle icons
var insertStyles = "Y.one(document.head).append('<style> \
  div.right-arrow {                                      \
    width: 0;                                            \
    height: 0;                                           \
    border-bottom: 5px solid transparent;                \
    border-top: 5px solid transparent;                   \
    border-left: 5px solid #356de4;                      \
    font-size: 0;                                        \
    margin-right: 5px;                                   \
    vertical-align: 5px;                                 \
    float: left;                                         \
  }                                                      \
  div.down-arrow {                                       \
    width: 0;                                            \
    height: 0;                                           \
    border-left: 5px solid transparent;                  \
    border-right: 5px solid transparent;                 \
    border-top: 5px solid #356de4;                       \
    font-size: 0;                                        \
    margin-right: 5px;                                   \
    float: left;                                         \
  }                                                      \
  div.upmGroup {                                         \
    font-weight: bold;                                   \
  }                                                      \
</style>');";


var scripts = "YUI().use('node', 'event', function (Y) {"
  + onClickHandler  
//  + insertStyles
  + "});";


module.exports = {
  
  listByCategory: function() {
    return listByGroup(this.modules, this.classes, 'Category', this.projectRoot);
  },

  listByManufacturer: function() {
    return listByGroup(this.modules, this.classes, 'Manufacturer', this.projectRoot);
  },

  listByConnection: function() {
    return listByGroup(this.modules, this.classes, 'Connection', this.projectRoot);
  },

  javascripts: function(options) {
    return '<script type="text/javascript">' + scripts + '</script>';
  },

  // generate custom cross links
  // assume lowercase script will be run, so generate class links with lower case
  customCrossLinks: function() {
    var selector = 'script[type="class-metadata"]'
    var html = "<script type='text/javascript'>                                    \
      var find = function(ar, elem) {                                              \
        for (var i = 0; i < ar.length; i++) {                                      \
          if (ar[i] == elem) {                                                     \
            return true;                                                           \
          }                                                                        \
        }                                                                          \
        return false;                                                              \
      };                                                                           \
      YUI().use('node', 'event', function (Y) {                                    \
        Y.on('domready', function() {                                              \
          var classes = Y.all('" + selector  + "').getHTML();                      \
          for (var i = 0; i < classes.length; i++) {                               \
            classes[i] = classes[i].toLowerCase();                                 \
          }                                                                        \
          Y.all('span.type').each(function(node) {                                 \
            var t = node.getHTML();                                                \
            if (find(classes, t.toLowerCase())) {                                  \
              node.setHTML('<a href=' + t.toLowerCase() + '.html>' + t + '</a>');  \
            }                                                                      \
          });                                                                      \
        });                                                                        \
      });                                                                          \
    </script>";
    for (var i = 0; i < this.classes.length; i++) {
      html += "<script type='class-metadata'>" + this.classes[i].name + "</script>";
    }
    return html;
  }

};