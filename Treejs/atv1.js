var parentPlanet1, parentPlanet2, renderer, scene, camera, controls, animationId;

function init()
{
	InitializeInfo();

	renderer = new THREE.WebGLRenderer();
	renderer.setSize(window.innerWidth, window.innerHeight);
	document.getElementById("outputGL").appendChild(renderer.domElement);

	scene = new THREE.Scene();

	camera = new THREE.PerspectiveCamera(90, window.innerWidth / window.innerHeight, 1, 100);
	camera.position.set(20, 60, 0);

	InitializeControls();

	//scene.add(new THREE.AxisHelper(20));
	
	//Parents
	parentPlanet1 = new THREE.Object3D();
	parentPlanet2 = new THREE.Object3D();
	
	parentPlanet1.position.set(0,0,0);
	parentPlanet2.position.set(0,0,0);
	parentPlanet1.rotation.set(0,0,0);
	parentPlanet2.rotation.set(0,0,0);
		
	InitializeScene();
	
	//Lines
	//var colorMaterial = [0xFFFFFF, 0x00FFFF, 0x8A2BE2, 0x7FFF00, 0x00FFFF, 0x9400D3, 0x90EE90, 0xF4A460];
	var lineMaterial = new THREE.LineBasicMaterial({color: controls.lineColor});
	var lineGeometry = new THREE.Geometry();
	
	animate();


	//
	//Functions Definitions
	function InitializeScene()
	{
		var planetObj1 = CreatePlanet(12, controls.radiusPlanet1, 0x6495ED);
		var planetObj2 = CreatePlanet(30, controls.radiusPlanet2, 0xCD5C5C);
		var sunObj = CreateSun(controls.radiusSun, 0xFF8C00);
				
		parentPlanet1.add(planetObj1);
		parentPlanet2.add(planetObj2);
		
		//Add to Scene
		scene.add(parentPlanet1);
		scene.add(parentPlanet2);
		scene.add(sunObj);
	};

	function CreateSun(radius, color)
	{
		var sun_sphere = new THREE.Object3D();
		sun_sphere.position.set(0, 0, 0);

		var meshSun = new THREE.Mesh(
			new THREE.SphereGeometry(radius, 10, 10),
			new THREE.MeshBasicMaterial({color: color, wireframe: false})
			);

		sun_sphere.add(meshSun);

		return sun_sphere;
	};

	function CreatePlanet(positionX, radius, color)
	{
		var planet = new THREE.Object3D();
		planet.position.x = positionX;

		var meshPlanet = new THREE.Mesh(
			new THREE.SphereGeometry(radius, 10, 10),
			new THREE.MeshBasicMaterial({color: color, wireframe: false})
		);

		planet.add(meshPlanet);

		return planet;
	};

	function animate()
	{		
		animationId = requestAnimationFrame(animate);
		//console.log(parentPlanet1.position, parentPlanet1.children[0].position, parentPlanet1.children[0].children[0].position);
		
		parentPlanet1.rotation.y += controls.speedPlanet1;
		parentPlanet2.rotation.y += controls.speedPlanet2;

		DrawNewLine();

		//console.log(camera.position);
		controls.update();
		renderer.render( scene, camera );
	};
	
	function DrawNewLine()
	{
		var childPosition1 = new THREE.Vector3();
		var childPosition2 = new THREE.Vector3();

		childPosition1.setFromMatrixPosition(parentPlanet1.children[0].matrixWorld);
		childPosition2.setFromMatrixPosition(parentPlanet2.children[0].matrixWorld);
	
		lineGeometry.vertices.push(childPosition1, childPosition2);
	
		var line = new THREE.Line(lineGeometry, lineMaterial);
		lineGeometry = new THREE.Geometry();
		scene.add(line);
					
	};

	function InitializeInfo()
	{
		info = document.createElement('div');
		info.style.position = 'absolute';
		info.style.top = '30px';
		info.style.width = '100%';
		info.style.textAlign = 'center';
		info.style.color = '#ffffff';
		info.style.fontWeight = 'bold';
		info.style.backgroundColor = 'transparent';
		info.style.zIndex = '1';
		info.style.fontFamily = 'Times';
		info.innerHTML = 'Arraste o mouse para rotacionar a camera; Scroll para zoom';
		document.body.appendChild(info);

		info = document.createElement('div');
		info.style.position = 'absolute';
		info.style.top = '30px';
		info.style.width = '100%';
		info.style.textAlign = 'left';
		info.style.color = '#ffffff';
		info.style.fontWeight = 'bold';
		info.style.backgroundColor = 'transparent';
		info.style.zIndex = '1';
		info.style.fontFamily = 'Times';
		info.innerHTML = 'Aluno: Evaldo M. Moreira Junior';
		document.body.appendChild(info);
	};

	function redraw()
	{
		if(animationId)
			cancelAnimationFrame(animationId);
		else
			return

		while(scene.children.length > 0)
    		scene.remove(scene.children[0]);

    	parentPlanet1.remove(parentPlanet1.children[0]);
    	parentPlanet2.remove(parentPlanet2.children[0]);

    	InitializeScene();
    	animate();
	};

	function ChangeLineColor()
	{
		if(animationId)
			cancelAnimationFrame(animationId);
		else
			return

		lineMaterial = new THREE.LineBasicMaterial({color: controls.lineColor});

		animate();
	};

	function InitializeControls()
	{
		controls = new THREE.OrbitControls(camera);
	    controls.minDistance = 10;	//Min zoom
	    controls.maxDistance = 90;	//Max zoom
	    controls.radiusSun = controls.radiusPlanet1 = controls.radiusPlanet2 = 2;
	    controls.speedPlanet1 = 0.079;
		controls.speedPlanet2 = 0.01;
		controls.lineColor = 0xFFFFFF;

		var gui = new dat.GUI();
	    //gui.add(controls, 'minDistance', 0, 10);
	    //gui.add(controls, 'maxDistance', 0, 50);
	    gui.add(controls, 'radiusSun', 0.1, 8).onChange(redraw);
	    gui.add(controls, 'radiusPlanet1', 0.1, 8).onChange(redraw);
	    gui.add(controls, 'radiusPlanet2', 0.1, 8).onChange(redraw);
	    gui.add(controls, 'speedPlanet1', 0, 0.1);
	    gui.add(controls, 'speedPlanet2', 0, 0.1);
	    gui.addColor(controls, 'lineColor').onChange(ChangeLineColor);
	};
};