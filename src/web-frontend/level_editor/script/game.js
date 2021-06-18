class Game 
{
    __construct(targetElem = null)
    {
        this.targetElem = targetElem;
    }

    load(leveDataJson)
    {
        Module.canvas = (function() { return document.getElementById('canvas'); })();
        Module._round = Math.round;
        Module.onRuntimeInitialized = (function(){
            const start_level = Module.cwrap("start_level", "number", ["string"]); 
            
            // const mockInput = JSON.stringify({
            //     test:"value",
            // });
            const retPtr = start_level(leveDataJson);
        });

        parent = this.targetElem ?? document.getElementById("view-game");
        //const parent = document.getElementById("view-game");
        const script = document.createElement("script");
        script.src = "script/wasm.js";
        parent.append(script);
    }
}

export {Game};