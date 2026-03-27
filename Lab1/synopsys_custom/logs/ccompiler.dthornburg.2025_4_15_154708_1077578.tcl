gi::pressButton {/scroll/scrollArea/scrollAreaInnerWidget/applications/dmLibraryManager} -in [gi::getWindows 1]
gi::setCurrentIndex {libs} -index {verilogaLib} -in [gi::getWindows 2]
gi::setItemSelection {libs} -index {verilogaLib} -in [gi::getWindows 2]
gi::setItemSelection {libs} -index {} -in [gi::getWindows 2]
gi::executeAction {menuPreShow} -in [gi::getWindows 2]
gi::executeAction {menuPreShow} -in [gi::getWindows 2]
de::showOpenDesign
gi::setActiveDialog [gi::getDialogs {deOpenDesign}]
db::setAttr geometry -of [gi::getDialogs {deOpenDesign}] -value 645x491+628+233
gi::pressButton {cancel} -in [gi::getDialogs {deOpenDesign}]
gi::executeAction {menuPreShow} -in [gi::getWindows 2]
db::setAttr iconified -of [gi::getFrames 1] -value true
