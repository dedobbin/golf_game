class Level 
{
	constructor(ctx, name, width, height, gravity)
	{
		this.ctx = ctx;
		this.name = name  ?? "";
		this.width = width ?? 5000;
		this.height = height ?? 1000;
		this.gravity = gravity ?? 0.65;

		this.entities = [];

		this.displayGrid = false;
		this.grid = {x: 10, y: 10};//0 or negative to turn off

		this.ctx.canvas.width = width;
		this.ctx.canvas.height = height;
	}

	_drawEntity(entity, x, y, useFallback = false)
	{
		const img = new Image();
		const sprite = entity.getSpriteSheetToDisplay();
		const ctx = this.ctx;

		img.onload = (function(){
			if (sprite && !useFallback){
				ctx.drawImage(img,
					 sprite.src[0], sprite.src[1], sprite.src[2], sprite.src[3]
					, x, y, entity.fields.size.w, entity.fields.size.h);
			} else {
				ctx.globalAlpha = 0.5;
				ctx.drawImage(img,
					0, 0, 32, 32
				   , x, y, entity.fields.size.w, entity.fields.size.h);
			   ctx.globalAlpha = 1;
			}
		});
		
		img.onerror = e =>{
			if (!useFallback){
				console.error("Couldn't load " + img.src + " using fallback..")
				this._drawEntity(entity,x, y, true);
			}
		}

		const file = !useFallback && sprite ? sprite.spritesheetName : "no_spritesheet";
		img.src = "resources/spritesheets/" + file + ".png";
	}

	_drawGrid()
	{
		if (this.grid.y > 0){
			const numHLines = this.height / this.grid.y;
			for (let i = 0; i < numHLines;i++){
				this.ctx.beginPath();
				this.ctx.moveTo(0, i * this.grid.y);
				this.ctx.lineTo(this.width, i * this.grid.y);
				this.ctx.stroke();
			}
		}

		if (this.grid.x > 0){
		const numVLines = this.width / this.grid.x;
			for (let i = 0; i < numVLines;i++){
				this.ctx.beginPath();
				this.ctx.moveTo(i * this.grid.x, 0);
				this.ctx.lineTo(i * this.grid.x, this.height);
				this.ctx.stroke();
			}
		}
	}

	_hasCollision(a, b){
		const leftA = a.x;
		const rightA = a.x + a.w;
		const topA = a.y;
		const bottomA = a.y + a.h;
	
		const leftB = b.x;
		const rightB = b.x + b.w;
		const topB = b.y;
		const bottomB = b.y + b.h;

		return  !(bottomA <= topB || topA >= bottomB || rightA <= leftB ||leftA >= rightB)
	}

	//Based on SDL_IntersectRect
	_getIntersect(a, b)
	{	
		const intersect = {x: 0, y: 0, w: 0, h: 0};
		/* Horizontal intersection */
		let Amin = a.x;
		let Amax = Amin + a.w;
		let Bmin = b.x;
		let Bmax = Bmin + b.w;
		if (Bmin > Amin)
			Amin = Bmin;
		intersect.x = Amin;
		if (Bmax < Amax)
			Amax = Bmax;
		intersect.w = Amax - Amin;

		/* Vertical intersection */
		Amin = a.y;
		Amax = Amin + a.h;
		Bmin = b.y;
		Bmax = Bmin + b.h;
		if (Bmin > Amin)
			Amin = Bmin;
		intersect.y = Amin;
		if (Bmax < Amax)
			Amax = Bmax;
		intersect.h = Amax - Amin;

		return intersect; 
	}

	addEntity(entity, x, y)
	{
		x = Math.floor(x / this.grid.x) * this.grid.x;
		y = Math.floor(y / this.grid.y) * this.grid.y;

		let hasCollision = false;
		if (entity.fields.collision?.solid){
			this.entities.forEach(possibleCollider=>{
				if (possibleCollider.entity.fields.collision?.solid){
					const posA = {
						x: possibleCollider.pos.x, 
						y: possibleCollider.pos.y, 
						w: possibleCollider.entity.fields.size.w, 
						h: possibleCollider.entity.fields.size.h
					};

					const posB = {
						x: x, 
						y: y, 
						w: entity.fields.size.w, 
						h: entity.fields.size.h
					};


					if (this._hasCollision(posA, posB)){
						console.log("DEBUG: has collision, won't draw")
						hasCollision = true;
						return
					}
				}
			});
		}
		if (hasCollision)
			return;

		//console.log("DEBUG: added entity: ",x, y, entity);
		this.entities.push({entity: entity, pos: {x: x,y: y}});
		this._drawEntity(entity, x, y);
	}

	eraseEntity(x, y)
	{
		//Inverted to erase entity at top first
		for(let i = this.entities.length-1; i >= 0 ; i--){
			const posA = {x: x, y: y, w: 1, h: 1};
			const posB = {
				x: this.entities[i].pos.x, 
				y: this.entities[i].pos.y, 
				w: this.entities[i].entity.fields.size.w, 
				h: this.entities[i].entity.fields.size.h
			};
			if (this._hasCollision(posA, posB)){
				this.entities.splice(i, 1);
				this.refreshCanvas(this.ctx);
				return;		
			}
		}
	}

	getEntity(x, y)
	{
		//Inverted to erase entity at top first
		for(let i = this.entities.length-1; i >= 0 ; i--){
			const posA = {x: x, y: y, w: 1, h: 1};
			const posB = {
				x: this.entities[i].pos.x, 
				y: this.entities[i].pos.y, 
				w: this.entities[i].entity.fields.size.w, 
				h: this.entities[i].entity.fields.size.h
			};
			if (this._hasCollision(posA, posB)){
				return this.entities[i]?.entity;	
			}
		}
	}

	clear()
	{	
		this.entities = [];
		this.refreshCanvas();
	}

	refreshCanvas()
	{
		this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height);
		
		if (this.displayGrid){
			this._drawGrid();
		}

		this.entities.forEach(e=>{
			this._drawEntity(e.entity, e.pos.x, e.pos.y);
		});
	}

	toJSON()
	{
		const result = {
			name: this.name,
			world_w: this.width,
			world_h: this.height,
			world_gravity: this.gravity
		};

		result.entities = this.entities.map(e=>{
			const entityData = JSON.parse(JSON.stringify(e.entity.fields));	//shallow copy so can unset .size before exporting
			//pos is special case
			entityData.pos = [e.pos.x, e.pos.y, e.entity.fields.size.w, e.entity.fields.size.h]; 
			delete entityData.size;
			//console.log("DEBUG exporting: ", entityData);
			return entityData;
		});

		//console.log("DEBUG: level export obj", result);
		return JSON.stringify(result);
	}
} 


export {Level}