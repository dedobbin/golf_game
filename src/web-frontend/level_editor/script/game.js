class Game 
{
    static load(_parent)
    {
        Module.canvas = (function() { return document.getElementById('canvas'); })();
        Module._round = Math.round;
        Module.onRuntimeInitialized = (function(){
            const start_level = Module.cwrap("start_level", null, ["number", "number"]); 
            var ptr = Module._malloc( 4);
            Module.setValue(ptr, 1, "i32");
            start_level(ptr, 1);
        });

        parent = _parent ?? document.getElementById("view-game");
        //const parent = document.getElementById("view-game");
        const script = document.createElement("script");
        script.src = "script/wasm.js";
        parent.append(script);
    }
}

export {Game};