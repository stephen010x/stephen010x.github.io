/*(async() => {
    // Define memory object
    const memory = new WebAssembly.Memory({initial: 256, maximum: 256}); 
    
    const importObject = {
        env: {
            memory: memory,
            print: console.log,
        },
    };

    const response = await fetch('bin/bin.wasm');
    const bytes = await response.arrayBuffer();
    const { instance } = await WebAssembly.instantiate(bytes, importObject);

    console.log('The answer is: ' + instance.exports.add(1, 2));
})();


/*
	// Import the C function
	const cFunction = Module.cwrap('cFunction', null,
	['number', 'string']);
*/
