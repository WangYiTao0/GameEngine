#define MaxLight 16




//Lambert's Cosine Law  
//E2 = E1(max(dot(n,L),0);
//L lightDir
//n normal
//E1 Radiant flux  while L == n

//diffuse albedo
//diffuse reflectance
// Cd = dot(max(dot(L,n),0),(BL*md))

//ambient light
//Ca  = AL * md

//speculaar reflection
//fresnel effect
// reflect  Rf 
// refract  (1 - Rf)
// index of refraction

// Rf(rad) = Rf(0') + (1-Rf(0'))pow((1-cos(Rad)),5); 

//roughness
//halfway vector 
// h = normalize(L+v);
// rad between  h and v
// p(rad) = cosm(rad) = cosm(n*h);
// spec(rad) = m + 8 /8 * p(rad) = m+8/8*pow((n*h),m);


//Cs = max(dot(n,L),0)*BL*
//      Rf(ƒ¿h)m+8/8*pow((n*h),m)

//lit Color = Ca + Cd + Cs
//