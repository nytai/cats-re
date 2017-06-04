import parseXml from "xml-parser"

export function gatherImages (xmlStr) {
  const obj = parseXml(xmlStr)
  return obj.root.children[0].children[0].children.map(x => x.children[0].content)
}
