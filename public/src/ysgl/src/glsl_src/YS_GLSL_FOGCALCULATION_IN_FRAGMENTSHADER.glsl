{
	MIDP  float d=fogDensity*abs(fogZ);
	MIDP  float f=clamp(exp(-d*d),0.0,1.0);
	LOWP  vec3  fogMix=mix(fogColor.rgb,gl_FragColor.rgb,f);
	gl_FragColor.rgb=fogMix;
}
