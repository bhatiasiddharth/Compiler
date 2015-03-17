var fs = require("fs");
var textToTree = require("./textToTree");
var treeToDiagram = require("./treetodiagram");
var jsdom = require("node-jsdom").jsdom;



function clear(node) {
  while (node.childNodes.length > 0)
    node.removeChild(node.childNodes[0]);
}

var options = {
 "flipXY": 0,
 "width": 1000,
 "height": 650,
 "labelLineSpacing": 12,
 "cornerRounding": 0,
 "labelPadding": 2,
 "arrowHeadSize": 4,
 "arrowsUp": 0,
 "siblingGap": 0.05,
 "idealSiblingGap": 0.3,
 "minimumCousinGap": 0.2,
 "idealCousinGap": 1.5,
 "levelsGap": 1,
 "minimumDepth": 6,
 "minimumBreadth": 6,
 "drawRoot": false
};

var styles = 'text {\n'+
  'text-anchor: middle;\n'+
  'font-size: x-small;\n'+
'}\n'+
'rect {\n'+
  'fill: aliceblue;\n'+
  'stroke: black;\n'+
  'opacity: 1;\n'+
  'stroke-width: 0.4;\n'+
  'stroke-dasharray: 4, 1;\n'+
'}\n'+
'line {\n'+
  'stroke: black;\n'+
  'opacity: 0.30;\n'+
  'stroke-width: 1;\n'+
'}\n';
  
var doc = '<svg id="diagramSvg" xmlns="http://www.w3.org/2000/svg">\n'+
      '<style id="stylesheet"> </style>\n'+
      '<defs>\n'+
        '<marker id="arrowHead" viewBox="-10 -5 10 10"\n'+
                'markerUnits="strokeWidth" markerWidth="6" markerHeight="5"\n'+
                'orient="auto">\n'+
          '<path d="M -10 -5 L 0 0 L -10 5 z"></path>\n'+
        '</marker>\n'+
      '</defs>\n'+
      '<g id="diagramGroup"></g>\n'+
    '</svg>\n';


function draw(treeInputFile, errors, window) {
  var doc = window.document;
  var treeInput = fs.readFileSync(treeInputFile, {encoding: 'utf8'});
  var tree = textToTree(treeInput);

  var diagramGroup = doc.getElementById('diagramGroup');
  var diagramSvg = doc.getElementById('diagramSvg');
  var arrowHead = doc.getElementById('arrowHead');
  var styleSheet = doc.getElementById("stylesheet");

  var treeOutputFile = treeInputFile.split('.')[0] + '.svg';
  clear(styleSheet);
  styleSheet.appendChild(doc.createTextNode(styles));
  // diagramSvg.style = {};
  clear(diagramGroup);
  treeToDiagram(doc, tree, diagramSvg, diagramGroup, options);

  fs.writeFileSync(treeOutputFile, '<svg id="diagramSvg" xmlns="http://www.w3.org/2000/svg">' + 
    window.document.querySelector('#diagramSvg').innerHTML + '</svg>');
}

jsdom.env({
    html : doc, 
    done : function(errors, window) {
      for (var i = 2; i < process.argv.length; i++) {
        draw(process.argv[i], errors, window);
      };
    }
});