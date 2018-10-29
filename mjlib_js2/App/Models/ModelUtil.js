var ModelUtil = module.exports;

ModelUtil.deepCopy = function (list) {
	if(list){
		var result = list.slice(0);
		return result;

	}
	return null;
	
}

ModelUtil.sort = function (list) {
	list.sort(function (a, b) {
		return (a.privatePoint - a.isGhost * 35) - (b.privatePoint - b.isGhost * 35);
	});
	return list;
}

ModelUtil.SAFE = function(otherValue,defVal){
	let result =  otherValue !== undefined ? otherValue :defVal;
	return result;
}


