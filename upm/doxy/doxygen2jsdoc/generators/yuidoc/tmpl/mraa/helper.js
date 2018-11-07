function getEnumType(t) {
  var pattern = new RegExp();
  var matched = t.match(/Enum [A-Za-z0-9\_\-]*/);
  if (matched) {
    return (matched[0].replace('Enum ', ''));
  } else {
    return null;
  }
}


function getType(property) {
  return property.type;
}


function getName(property) {
  return property.name;
}


function getDescription(property) {
  return property.propertyDescription.replace(/\<p\>|\<\/p\>|\\n/gi, '');
}


function groupByType(properties) {
  var byType = {};
  for (var i = 0; i < properties.length; i++) {
    var enumType = getEnumType(getType(properties[i]));
    if (enumType) {
      var e = {
        name: getName(properties[i]),
        description: getDescription(properties[i])
      };
      if (enumType in byType) {
        byType[enumType].push(e);
      } else {
        byType[enumType] = [e];
      }
    }
  }
  return byType;
}


var scriptPrefix = "<script type='text/javascript'>      \
  YUI().use('node', 'datatable', function (Y) {"


var scriptPostfix = "                                    \
  });                                                    \
</script>";


var insertStyles = "Y.one(document.head).append('<style> \
  div.no-visible-items {                                 \
    display: none;                                       \
  }                                                      \
  table.yui3-datatable-table {                           \
    width: auto;                                         \
  }                                                      \
</style>');";


function createTable(enumType, enums) {
  var html = '<div class="item">                                                                                      \
    <div id="enum_' + enumType + '" style="margin-top:-0.75em; margin-bottom:1.5em"><b>' + enumType + '</b></div>' + '\
    <div class="yui3-skin-sam">                                                                                       \
      <table class="yui3-datatable-table">                                                                            \
        <thead class="yui3-datatables-columns">                                                                       \
          <th class="yui3-datatable-header">Enum</th>                                                                 \
          <th class="yui3-datatable-header">Description</th>                                                          \
        </thead>                                                                                                      \
        <tbody class="yui3-datatable-data">';
  for (var i = 0; i < enums.length; i++) {
    html += '                                                                                                         \
          <tr class="' + (((i % 2) == 0) ? 'yui3-datatable-even' : 'yui3-datatable-odd') + '">                        \
            <td class="yui3-datatable-cell">' + enums[i].name + '</td>' + '                                           \
            <td class="yui3-datatable-cell">' + enums[i].description + '</td>                                         \
          </tr>';
  }
  html += '                                                                                                           \
        </tbody>                                                                                                      \
      </table>                                                                                                        \
    </div>                                                                                                            \
  </div>';
  return html;
}


module.exports = {
  
  listEnumGroups: function(properties) {
    var enumsByType = groupByType(properties);
    var html = '';
    for (var enumType in enumsByType) {
      html += '<li class="index-item property"><a href="#enum_' + enumType + '">' + enumType + '</a></li>';
    }
    return html;
  },
  
  listEnums: function(properties) {
    var enumsByType = groupByType(properties);
    var html = ''
    var scripts = scriptPrefix;
    for (var enumType in enumsByType) {
      html += createTable(enumType, enumsByType[enumType]);
    }
    scripts += insertStyles + scriptPostfix;
    return html + scripts;
  },
  
  enumCrossLinks: function() {
    var html = "<script type='text/javascript'>"  +
      getEnumType.toString() + "                                                       \
      YUI().use('node', 'event', function (Y) {                                        \
        Y.on('domready', function() {                                                  \
          Y.all('span.type').each(function(node) {                                     \
            var t = node.getHTML();                                                    \
            var enumType = getEnumType(t);                                             \
            if (enumType) {                                                            \
              node.setHTML('<a href=common.html#enum_' + enumType + '>' + t + '</a>'); \
            }                                                                          \
          });                                                                          \
        });                                                                            \
      });                                                                              \
    </script>";
    return html;
  }
  
};