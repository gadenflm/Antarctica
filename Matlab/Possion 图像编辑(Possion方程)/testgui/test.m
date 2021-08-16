function varargout = test(varargin)
% TEST MATLAB code for test.fig
%      TEST, by itself, creates a new TEST or raises the existing
%      singleton*.
%
%      H = TEST returns the handle to a new TEST or the handle to
%      the existing singleton*.
%
%      TEST('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TEST.M with the given input arguments.
%
%      TEST('Property','Value',...) creates a new TEST or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before test_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to test_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help test

% Last Modified by GUIDE v2.5 19-Dec-2020 23:22:14

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @test_OpeningFcn, ...
                   'gui_OutputFcn',  @test_OutputFcn, ...
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


% --- Executes just before test is made visible.
function test_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to test (see VARARGIN)

% Choose default command line output for test
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes test wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = test_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
[fname fpath]=uigetfile('*.*','Please open your background image file.');
path=[fpath fname];

global bimg_data;
bimg_data=double(imread(path))/255;
axes(handles.bimg);
hold off;
handles.im=imshow(bimg_data);

axis on
set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'XColor',[1 1 1]);
set(handles.bimg,'YColor',[1 1 1]);
set(handles.bimg,'FontSize',0.001);

set(handles.im,'Pickableparts','none');
set(handles.bimg,'ButtonDownFcn',@mycallbackfcn);

global pos_int;
pos_int=0;
 
function mycallbackfcn(hObject,eventdata,handles)
handles = guidata(hObject);

global pos_int;
global pos_total;
pos_int=pos_int+1;

pos = get(hObject,'Currentpoint');

pos_total(pos_int,1)=pos(1,1) ;
pos_total(pos_int,2)=pos(1,2) ;



if (pos_int~=1)
axes(handles.bimg);
hold on;
plot ( [pos_total(pos_int-1,1),pos_total(pos_int,1)] , [pos_total(pos_int-1,2),pos_total(pos_int,2)] ,'w');

end


% --- Executes on mouse press over axes background.
function bimg_ButtonDownFcn(hObject, eventdata, handles)


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)

global pos_int;
global pos_total;

for i=1: (pos_int-1)
    g(i)= pos_total(i,1);
    f(i) = pos_total(i,2);
end

global bimg_data;
mask_temp=roipoly( bimg_data,g,f);
mask(:,:,1)=mask_temp;
mask(:,:,2)=mask_temp;
mask(:,:,3)=mask_temp;
out_data=mask.*bimg_data;

axes(handles.bimg);
handles.im=imshow(out_data);

