///////////////////////////////////////////////////////////////////////
///////////////////////////BOILER PLATE TO ADD SOMEWHERE ON THE JS FILE
///////////////////////////////////////////////////////////////////////
var __magic_pgo_state__ = {total_number:0, report:100, last:-1, flagIsIf:-1, map:new Map()};
function __pgo__processLast(taken)
{
	__magic_pgo_state__.total_number++;

	if (!__magic_pgo_state__.map.has(__magic_pgo_state__.last))
		__magic_pgo_state__.map.set(__magic_pgo_state__.last, {times:0, taken:0, isIf:__magic_pgo_state__.flagIsIf});

	var data = __magic_pgo_state__.map.get(__magic_pgo_state__.last);
	data.times++;
	if (taken)
		data.taken++;

	__magic_pgo_state__.last = -1;
	__magic_pgo_state__.flagIsIf = -1;
}

function __magic_pgo_function__(a)
{
	if (__magic_pgo_state__.last == -1)
		__magic_pgo_state__.last = a;
	else if (__magic_pgo_state__.flagIsIf == -1)
		__magic_pgo_state__.flagIsIf = a;
	else
	{
		__pgo__processLast(a == -1);
		if (a != -1)
			__magic_pgo_function__(a);
	}
}

function __getHint(num, den)
{
	if (den < 20)
		return 2; //NO HINT if visited too few times
	if (num / den > 0.9)
		return 1; //DO TAKE if taken > 90%
	if (num / den < 0.1)
		return 0; //DO NOT TAKE if taken < 10%

	return 2; //NO HINT in all other cases
}
function __magic_pgo_report__()
{
	var array = new Array();

	for (const [key, value] of __magic_pgo_state__.map.entries()) {
		var den = value.times;
		var num = value.taken;

		//BrIf are encoded the reverse way
		if (!value.isIf)
			num = den - num;

		array.push({id: key, hint: __getHint(num, den)});
	}
	array = array.sort(function(a, b){
			return a.id - b.id;
			});

	var hints = new Array();
	var i = 0;
	var j = 0;
	while (j < array.length)
	{
		while (array[j].id > i++)
			hints.push(2);	//NO HINT if no other information
		hints.push(array[j].hint);
		i++;
		j++;
	}

	var res = "";
	for (const hint of hints)
	{
		res += hint;
	}
	console.log(res);
}
//////////////////////////////////////////////////////////////////



//Example with Cheerp generated files, but shoud works with any other instantiate strategy

__dummy.promise=
fetchBuffer('anotherAnnotate.wasm').then(Lp=>
WebAssembly.instantiate(Lp,
{i:{
		_abort:_abort,
		___wrapper___Z7printlnPKcj:___wrapper___Z7printlnPKcj,
		growLinearMemory:growLinearMemory,
		__magic_pgo_function__:__magic_pgo_function__,	//<--- Add this additional import
		}})
).then(Lp=>{
	__asm=Lp.instance.exports;
	__heap=__asm.memory.buffer;
	assignHeaps(__heap);
	_free=__asm._free;
	__asm._main();
	__magic_pgo_report__();			// <---- Add this after all computations are computed
});
