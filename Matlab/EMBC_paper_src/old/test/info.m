%laminar flow (low Reynolds number)
%turbulent flow (high Reynolds number)

%Reynolds number Re:
reynolds_num = (fluid_density * fluid_velocity * vessels_radius) / fluid_viscosity ;


%The velocity profile of such laminar flow vp(r):
vilosity_profile(r) = (1/ 4*fluid_viscosity ) * ( pressure_decrease / vessels_length ) * ( R ^ 2 - r ^ 2 ) ;

% The drag force of flow Fd:
flow_drag_force =  6 * pi * fluid_viscosity * particle_radius * vilosity_profile(r);
%Through this drag force value, it is possible to estimate both acceleration and velocity of particles.

%Brownian diffusion:  diffusion constant D:
D =  ( Boltzmann_constant * absolute_temperature ) / ( 6 * pi * fluid_viscosity * particle_radius );

%!!!!!!!!!!!!!!!
%( ignore the blood flow ,means the drag force of blood flow Fd = 0,)
%The basic one-dimensional Langevin equation for Browaian motion:
%The force of diffusion:
f(t) = particle_mass * ( diff(diff(x)) / ( diff(t)*diff(t) ) ) + 6* pi * fluid_viscosity * particle_radius * ( diff(x) / diff (t) );

%Stokes Law: the fiction coefficient Gamma:
gamma =  6* pi * fluid_viscosity * particle_radius;

%!!!!!!!!!!!!!!!!!!!!!!!!
%The force f(t) determines the diffusion process, the noise term is white Gaussian noise

%generalized Langevin equation for blood vessels model:
particle_mass * ( diff(diff(x)) / ( diff(t)*diff(t) ) ) + Gamma * ( diff(x) / diff (t) ) =
Gamma * vilosity_profile(r) + 2 * Gamma * Boltzmann_constant * absolute_temperature *   ¦Ä function form of the correlations