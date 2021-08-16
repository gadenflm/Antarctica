function varargout = widget(varargin)

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @widget_OpeningFcn, ...
    'gui_OutputFcn',  @widget_OutputFcn, ...
    'gui_LayoutFcn',  [] , ...
    'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before widget is made visible.
function widget_OpeningFcn(hObject, eventdata, handles, varargin)

handles.output = hObject;
guidata(hObject, handles);
wid_init(hObject, eventdata, handles);


% UIWAIT makes widget wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = widget_OutputFcn(hObject, eventdata, handles)
varargout{1} = handles.output;

%widget initial:
function wid_init(hObject, eventdata, handles)

set(handles.btn_openbimg,'Enable','on');
set(handles.btn_openfimg,'Enable','off');
set(handles.btn_genmask,'Enable','off');
set(handles.btn_poisson,'Enable','off');
set(handles.btn_mask_size_up,'Enable','off');
set(handles.btn_mask_size_down,'Enable','off');

set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'FontSize',0.001);

set(handles.fimg,'XColor',[1 1 1]);
set(handles.fimg,'YColor',[1 1 1]);
set(handles.fimg,'XColor',[1 1 1]);
set(handles.fimg,'YColor',[1 1 1]);
set(handles.fimg,'FontSize',0.001);

set(handles.maskimg,'XColor',[1 1 1]);
set(handles.maskimg,'YColor',[1 1 1]);
set(handles.maskimg,'XColor',[1 1 1]);
set(handles.maskimg,'YColor',[1 1 1]);
set(handles.maskimg,'FontSize',0.001);

set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'FontSize',0.001);

% --- Executes on button press in btn_openbimg.
function btn_openbimg_Callback(hObject, eventdata, handles)
[fname,fpath]=uigetfile('*.*','Please open your background image file.');
path=[fpath fname];
global bimg_data;
bimg_data=double(imread(path))/255;
axes(handles.bimg);
hold off;
handles.bim_show=imshow(bimg_data);

axis on
set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'FontSize',0.001);

set(handles.btn_openfimg,'Enable','on');


% --- Executes on button press in btn_openfimg.
function btn_openfimg_Callback(hObject, eventdata, handles)
[fname,fpath]=uigetfile('*.*','Please open your foreground image file.');
path=[fpath fname];
global fimg_data;
fimg_data=double(imread(path))/255;
axes(handles.fimg);
hold off;
handles.fim_show=imshow(fimg_data);

axis on
set(handles.fimg,'XColor',[1 1 1]);
set(handles.fimg,'YColor',[1 1 1]);
set(handles.fimg,'XColor',[1 1 1]);
set(handles.fimg,'YColor',[1 1 1]);
set(handles.fimg,'FontSize',0.001);

set(handles.fim_show,'Pickableparts','none');
set(handles.fimg,'ButtonDownFcn',@fpos_fun);
global fpos_index;
fpos_index=0;

set(handles.btn_genmask,'Enable','off');
set(handles.btn_poisson,'Enable','off');
set(handles.btn_mask_size_up,'Enable','off');
set(handles.btn_mask_size_down,'Enable','off');

% get Currentpoint
function fpos_fun(hObject,eventdata,handles)
handles = guidata(hObject);

global fpos_index;
global fpos_data;
fpos_index=fpos_index+1;

pos = get(hObject,'Currentpoint');

fpos_data(fpos_index,1)=floor ( pos(1,1) )+1;
fpos_data(fpos_index,2)=floor ( pos(1,2) ) +1;

if (fpos_index>1)
    axes(handles.fimg);
    hold on;
    plot ( [fpos_data(fpos_index-1,1),fpos_data(fpos_index,1)] , [fpos_data(fpos_index-1,2),fpos_data(fpos_index,2)] ,'w');
end

if(fpos_index==4)
    set(handles.btn_genmask,'Enable','on');
end

% --- Executes on button press in btn_genmask.
function btn_genmask_Callback(hObject, eventdata, handles)

global fpos_index;
global fpos_data;
global maskb;
global maskf;
global posb1;
global posb2;

posb1=1;
posb2=1;

for i=1: (fpos_index-1)
    g(i)= fpos_data(i,1);
    f(i) = fpos_data(i,2);
end

global bimg_data;
global fimg_data;

maskb_temp=roipoly( bimg_data,g+posb2,f+posb1);
maskf_temp=roipoly( fimg_data,g,f);
maskb=[];
maskf=[];

maskb(:,:,1)=maskb_temp;
maskb(:,:,2)=maskb_temp;
maskb(:,:,3)=maskb_temp;

maskf(:,:,1)=maskf_temp;
maskf(:,:,2)=maskf_temp;
maskf(:,:,3)=maskf_temp;

out_imshow(hObject, eventdata, handles);


% show out image
function out_imshow(hObject, eventdata, handles)

global bimg_data;
global fimg_data;
global maskb;
global maskf;

axes(handles.maskimg);
hold off;
handles.maskim_show=imshow(maskb);

axis on
set(handles.maskimg,'XColor',[1 1 1]);
set(handles.maskimg,'YColor',[1 1 1]);
set(handles.maskimg,'XColor',[1 1 1]);
set(handles.maskimg,'YColor',[1 1 1]);
set(handles.maskimg,'FontSize',0.001);

axes(handles.outimg);
hold off;

temp=maskf.*fimg_data;
tempb=(~maskb).*bimg_data;
tempimg=tempb;

global posb1;
global posb2;

tempimg( (posb1 : (posb1+size(temp,1)-1))  , (posb2 : (posb2+size(temp,2)-1)) ,:)=tempimg( (posb1 : (posb1+size(temp,1)-1)) , (posb2 : (posb2+size(temp,2)-1)) ,:)+temp;

handles.outim_show=imshow(tempimg);

axis on
set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'FontSize',0.001);

set(handles.outim_show,'Pickableparts','none');
set(handles.outimg,'ButtonDownFcn',@outpos_fun);

set(handles.btn_poisson,'Enable','on');
set(handles.btn_mask_size_up,'Enable','on');
set(handles.btn_mask_size_down,'Enable','on');


% detect mouse_dow
function outpos_fun(hObject,eventdata,handles)

 handles = guidata(hObject);
pos = get(hObject,'Currentpoint');

global fpos_index;
global fpos_data;
global posb1;
global posb2;

posb1=floor(pos(1,2))+1;
posb2=floor(pos(1,1))+1;

for i=1: (fpos_index-1)
    g(i)= fpos_data(i,1);
    f(i) = fpos_data(i,2);
end

global bimg_data;
global fimg_data;
global maskb;
global maskf;

maskb_temp=roipoly( bimg_data,g+posb2,f+posb1);
maskf_temp=roipoly( fimg_data,g,f);
maskb=[];
maskf=[];

maskb(:,:,1)=maskb_temp;
maskb(:,:,2)=maskb_temp;
maskb(:,:,3)=maskb_temp;

maskf(:,:,1)=maskf_temp;
maskf(:,:,2)=maskf_temp;
maskf(:,:,3)=maskf_temp;

out_imshow(hObject, eventdata, handles);

% --- Executes on button press in btn_poisson.
function btn_poisson_Callback(hObject, eventdata, handles)
global bimg_data;
global fimg_data;
global maskb;
global maskf;


F(:,:,1) = poisson_gray( bimg_data(:,:,1), fimg_data(:,:,1),maskb(:,:,1), maskf(:,:,1));
F(:,:,2) = poisson_gray( bimg_data(:,:,2), fimg_data(:,:,2),maskb(:,:,2), maskf(:,:,2));
F(:,:,3) = poisson_gray( bimg_data(:,:,3), bimg_data(:,:,3), maskb(:,:,3), maskf(:,:,3));

global outimg_data;
outimg_data=F;
handles.outim_show=imshow(outimg_data);

axis on
set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'XColor',[1 1 1]);
set(handles.outimg,'YColor',[1 1 1]);
set(handles.outimg,'FontSize',0.001);

fi2=figure();
imshow(outimg_data);

% --- Executes on button press in btn_mask_size_up.
function btn_mask_size_up_Callback(hObject, eventdata, handles)
global fpos_index;
global fpos_data;
global maskb;
global maskf;
global posb1;
global posb2;

for i=1: (fpos_index-1)
    g(i)= fpos_data(i,1)*1.25;
    fpos_data(i,1)=g(i);
    f(i) = fpos_data(i,2)*1.25;
    fpos_data(i,2)=f(i);
end

global bimg_data;
global fimg_data;
fimg_data=imresize(fimg_data, 1.25);

maskb_temp=roipoly( bimg_data,g+posb2,f+posb1);
maskf_temp=roipoly( fimg_data,g,f);
maskb=[];
maskf=[];

maskb(:,:,1)=maskb_temp;
maskb(:,:,2)=maskb_temp;
maskb(:,:,3)=maskb_temp;

maskf(:,:,1)=maskf_temp;
maskf(:,:,2)=maskf_temp;
maskf(:,:,3)=maskf_temp;

out_imshow(hObject, eventdata, handles);


% --- Executes on button press in btn_mask_size_down.
function btn_mask_size_down_Callback(hObject, eventdata, handles)
global fpos_index;
global fpos_data;
global maskb;
global maskf;
global posb1;
global posb2;

for i=1: (fpos_index-1)
    g(i)= fpos_data(i,1)*0.8;
    fpos_data(i,1)=g(i);
    f(i) = fpos_data(i,2)*0.8;
    fpos_data(i,2)=f(i);
end

global bimg_data;
global fimg_data;
fimg_data=imresize(fimg_data, 0.8);

maskb_temp=roipoly( bimg_data,g+posb2,f+posb1);
maskf_temp=roipoly( fimg_data,g,f);
maskb=[];
maskf=[];

maskb(:,:,1)=maskb_temp;
maskb(:,:,2)=maskb_temp;
maskb(:,:,3)=maskb_temp;

maskf(:,:,1)=maskf_temp;
maskf(:,:,2)=maskf_temp;
maskf(:,:,3)=maskf_temp;

out_imshow(hObject, eventdata, handles);
