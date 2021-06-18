class Game 
{
    static load(_parent)
    {
        Module.canvas = (function() { return document.getElementById('canvas'); })();
        Module._round = Math.round;
        Module.onRuntimeInitialized = (function(){
            const start_level = Module.cwrap("start_level", "number", ["string"]); 
            
            const mockInput = JSON.stringify({
                test:"value",
            });
            const retPtr = start_level(mockInput);
        });

        parent = _parent ?? document.getElementById("view-game");
        //const parent = document.getElementById("view-game");
        const script = document.createElement("script");
        script.src = "script/wasm.js";
        parent.append(script);
    }
}

export {Game};