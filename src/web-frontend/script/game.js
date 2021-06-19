class Game 
{
    __construct(targetElemName = null)
    {
        this.targetElemName = targetElemName;
    }

    load(leveDataJson)
    {
        if (typeof Module === 'undefined'){
            //Module should be decared in script in targetElems HTML, so glue code can reach it
            console.log("No WASM module found..");
            return false;
        }

        Module.canvas = (function() { return document.getElementById('canvas'); })();
        Module._round = Math.round;
        Module.onRuntimeInitialized = (function(){
            const start_level = Module.cwrap("start_level", "number", ["string"]); 
            const retPtr = start_level(leveDataJson);
        });

        parent = this.targetElemName ? document.getElementById(targetElemName) : document.getElementById("view-game");
        //const parent = document.getElementById("view-game");
        const script = document.createElement("script");
        script.src = "script/wasm.js"; //glue code
        parent.append(script);

        return true;
    }
}

export {Game};