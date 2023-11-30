--
-- PostgreSQL database dump
--

-- Dumped from database version 15.3 (Debian 15.3-0+deb12u1)
-- Dumped by pg_dump version 15.3 (Debian 15.3-0+deb12u1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: c_u_h; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.c_u_h (
    id integer NOT NULL,
    chip_time time without time zone,
    place integer,
    contest_id integer,
    horse_id integer,
    jockey_id integer
);


ALTER TABLE public.c_u_h OWNER TO server;

--
-- Name: c_u_h_id_seq; Type: SEQUENCE; Schema: public; Owner: server
--

CREATE SEQUENCE public.c_u_h_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.c_u_h_id_seq OWNER TO server;

--
-- Name: c_u_h_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: server
--

ALTER SEQUENCE public.c_u_h_id_seq OWNED BY public.c_u_h.id;


--
-- Name: contests; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.contests (
    id integer NOT NULL,
    name text NOT NULL,
    date date NOT NULL,
    "time" time without time zone NOT NULL,
    hippodrome_id integer,
    status text,
    info text
);


ALTER TABLE public.contests OWNER TO server;

--
-- Name: contests_id_seq; Type: SEQUENCE; Schema: public; Owner: server
--

CREATE SEQUENCE public.contests_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contests_id_seq OWNER TO server;

--
-- Name: contests_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: server
--

ALTER SEQUENCE public.contests_id_seq OWNED BY public.contests.id;


--
-- Name: hippodrome; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.hippodrome (
    id integer NOT NULL,
    name text NOT NULL,
    address text NOT NULL,
    description text NOT NULL
);


ALTER TABLE public.hippodrome OWNER TO server;

--
-- Name: hippodrome_id_seq; Type: SEQUENCE; Schema: public; Owner: server
--

CREATE SEQUENCE public.hippodrome_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.hippodrome_id_seq OWNER TO server;

--
-- Name: hippodrome_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: server
--

ALTER SEQUENCE public.hippodrome_id_seq OWNED BY public.hippodrome.id;


--
-- Name: horses; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.horses (
    id integer NOT NULL,
    name text NOT NULL,
    sex text NOT NULL,
    age integer NOT NULL,
    owner_id integer,
    available boolean
);


ALTER TABLE public.horses OWNER TO server;

--
-- Name: horses_id_seq; Type: SEQUENCE; Schema: public; Owner: server
--

CREATE SEQUENCE public.horses_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.horses_id_seq OWNER TO server;

--
-- Name: horses_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: server
--

ALTER SEQUENCE public.horses_id_seq OWNED BY public.horses.id;


--
-- Name: status; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.status (
    id integer NOT NULL,
    name character(20) NOT NULL
);


ALTER TABLE public.status OWNER TO server;

--
-- Name: users; Type: TABLE; Schema: public; Owner: server
--

CREATE TABLE public.users (
    id integer NOT NULL,
    name text NOT NULL,
    surname text NOT NULL,
    age integer NOT NULL,
    phone text NOT NULL,
    login text NOT NULL,
    password text NOT NULL,
    status_id integer,
    ltok text,
    address text
);


ALTER TABLE public.users OWNER TO server;

--
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: server
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO server;

--
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: server
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- Name: c_u_h id; Type: DEFAULT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.c_u_h ALTER COLUMN id SET DEFAULT nextval('public.c_u_h_id_seq'::regclass);


--
-- Name: contests id; Type: DEFAULT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.contests ALTER COLUMN id SET DEFAULT nextval('public.contests_id_seq'::regclass);


--
-- Name: hippodrome id; Type: DEFAULT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.hippodrome ALTER COLUMN id SET DEFAULT nextval('public.hippodrome_id_seq'::regclass);


--
-- Name: horses id; Type: DEFAULT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.horses ALTER COLUMN id SET DEFAULT nextval('public.horses_id_seq'::regclass);


--
-- Name: users id; Type: DEFAULT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- Data for Name: c_u_h; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.c_u_h (id, chip_time, place, contest_id, horse_id, jockey_id) FROM stdin;
2	\N	\N	1	2	4
1	\N	\N	1	1	3
5	00:07:49	1	2	2	4
6	00:10:15	2	2	1	3
7	\N	\N	1	4	10
8	\N	\N	1	3	7
9	\N	\N	1	5	9
\.


--
-- Data for Name: contests; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.contests (id, name, date, "time", hippodrome_id, status, info) FROM stdin;
1	First place	2024-08-13	04:05:00	1	open	\N
2	Go, Horse, go!	2023-10-08	16:00:00	1	closed	added
3	Horses Best	2024-09-27	14:00:00	1	открыта	\N
\.


--
-- Data for Name: hippodrome; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.hippodrome (id, name, address, description) FROM stdin;
1	FIREST	Moscow, Pushkinskaya	The best place ever!
\.


--
-- Data for Name: horses; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.horses (id, name, sex, age, owner_id, available) FROM stdin;
3	Дженифер	Кобыла	4	5	t
1	Звездочка	Кобыла	4	2	t
2	Снежок	Конь	3	2	t
4	Геральд	Конь	6	5	t
5	Лютик	Конь	7	5	t
6	Крот	Конь	7	2	t
7	Яков	Конь	3	2	f
\.


--
-- Data for Name: status; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.status (id, name) FROM stdin;
1	jockey              
2	owner               
3	admin               
\.


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: server
--

COPY public.users (id, name, surname, age, phone, login, password, status_id, ltok, address) FROM stdin;
4	Puf	Paf	24	88005553535	Joc2	9bf3e9aaf7fc2b681bc35ba20a56548ff8243875b0f3f596ea62096179fc5c8da33d5ecb46c5e7589e4862984581ea9ecccf82d28b7c624a95cbdf868e57b8be	1	52088548358445939732933011499646565121108263588834830151669768382548330118604&95571432379102826517008476973899108597083054732993073136079709076567996989602	Moscow, Tverskaya, 59
3	Pif	Paf	24	88005553535	Joc	bf03373e69e85680e9107d48d6ccdf1e24fd43e2d856344d2343da326fbb648cc24ff3561190ab76039c5514c577636937b96290870246dc093c96b23a214cfc	1	29710724564632896406885377667307844310490035431975915361570440510729680216018&70643158879479893694906167689471590842014676275501132664906429709602703146716	Moscow, Petrovskaya, 23
2	Do	Do	54	88005553535	Ow_no	32aa7e50f8ba3be4c70b709e668c0cc97d697d186298e54c8f3cb824725acdeb75504671975a14d827c054c323a32d3a7fbc089da862cdf0a5d0bcaa343b58ce	2	37933723527733971727394053765378098811346127623610095932557840662314433519611&103858603742180442242452770148394806781457607031115808422812651352914117666151	Saint-Petersburg, Dybenko, 10
1	Oleg	Dirty	34	88005553535	Oleg_d	0470cd9423169c9b6905067c97532f6d4ad441c718312019f7475f496f94bd933139774ce7ce78dba257c07e4af890f6f3e1d8ff6f1b17c5a917d5754fd50bd2	3	58367271790188533405173407880444155417988723345925860183156292914615103317543&93557157179367912049652717636510653901295721418986390458079133312316274894488	Novosibirsk, Gogol, 12
5	Du	Al	45	88005553535	ohno	acb0b4cdebe21c471e2dcc9be22735a753f7edfd699a2d9a4bdc05518401990748251c12574397d56d07bdf37a2928d2353cd5c4db578f3051eac7849634b6f6	2	6632111204988315641213228531499357436229325809403701864719692592656471230444&25326816162378819286545022686334726922890005669446121599965420697147755379202	Sahalinsk, Gagarin, 7
6	Сан	Саныч	56	88005553535	Joc3	c94da0527adf2b75cb54b7f30bccee396cc9289f4b7837a8842e7f847d7189a66481a03720babdbb4f918855479aad554006848ee5cfbede0d8f34197c136686	1	57386855473765497151803417698816471211103863056405781562239603686465020979666&62475021370182519571090491431704298293864271148159246581820802146131382479989	Москва, Пушкина, 7
7	Сан	Палыч	31	88005553535	Joc4	23f9a1001326f18eab39e6c561b78142cf986b65acc1ad2a9d168aba6c36d38da5730eb208c7bebc5d9ce12d758a8e2dccb0b39460383a194c56648efeb19fc3	1	34793989391197949149073823616870549336676392370899623343427955448845822964372&93671060964530849222426355478046669534980064081341642840462984635917793472177	Москва, Пушкина, 9
8	Иван	Палыч	33	88005553535	MrProper	526cbe05d8b9b644c319735a0cc7dbd07f363bd4d0dd3d6ffcfd6de1f5cf89483f27a237746c52d36995c79dc122a7eff6e7c1e6a9f7e60dd4bf3543e33a444e	1	108377373257124383756353788761962151552628840375755212689079959530634235906780&9780159766477409273910801651304108749108190422284826183092808451941740810543	Москва, Тверская, 89
9	Иван	Михалыч	20	88005553535	PiyPiy	44587687c80979d59f6ba2169b043d2be1e290aa8c0068bc4f16b186bb5da9d05f8a371fd51cd126ad1d06430898ac9f74a53d48b91040b067ca1eaca35dec1e	1	3764222143629347526157262207130950415144977796801874995954204420701300145970&82243083965429176211452817807872685507158156153392263568129332298373107812295	Москва, Скудная, 5
10	Олег	Грязь	31	88005553535	LisaLisa	828fd5981b33f16d0c3e19b95acc7a36e608f578975af8e969323bfcddffbbbbdf5403c11fee8607f1bf84e57be8f4657c9dee3b18c98b64b7663d9629d56656	1	33377799271553325024409104842395608980167680478077059948369948261639792004383&109554916784570073974933658659026726591929811574826193101606868203920878416598	Помойка, Пятерочка, 10
\.


--
-- Name: c_u_h_id_seq; Type: SEQUENCE SET; Schema: public; Owner: server
--

SELECT pg_catalog.setval('public.c_u_h_id_seq', 9, true);


--
-- Name: contests_id_seq; Type: SEQUENCE SET; Schema: public; Owner: server
--

SELECT pg_catalog.setval('public.contests_id_seq', 3, true);


--
-- Name: hippodrome_id_seq; Type: SEQUENCE SET; Schema: public; Owner: server
--

SELECT pg_catalog.setval('public.hippodrome_id_seq', 33, true);


--
-- Name: horses_id_seq; Type: SEQUENCE SET; Schema: public; Owner: server
--

SELECT pg_catalog.setval('public.horses_id_seq', 7, true);


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: server
--

SELECT pg_catalog.setval('public.users_id_seq', 10, true);


--
-- Name: contests contests_pkey; Type: CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.contests
    ADD CONSTRAINT contests_pkey PRIMARY KEY (id);


--
-- Name: hippodrome hippodrome_pkey; Type: CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.hippodrome
    ADD CONSTRAINT hippodrome_pkey PRIMARY KEY (id);


--
-- Name: horses horses_pkey; Type: CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.horses
    ADD CONSTRAINT horses_pkey PRIMARY KEY (id);


--
-- Name: status status_pkey; Type: CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.status
    ADD CONSTRAINT status_pkey PRIMARY KEY (id);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- Name: c_u_h fk_contest; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.c_u_h
    ADD CONSTRAINT fk_contest FOREIGN KEY (contest_id) REFERENCES public.contests(id);


--
-- Name: c_u_h fk_horse; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.c_u_h
    ADD CONSTRAINT fk_horse FOREIGN KEY (horse_id) REFERENCES public.horses(id);


--
-- Name: c_u_h fk_jockey; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.c_u_h
    ADD CONSTRAINT fk_jockey FOREIGN KEY (jockey_id) REFERENCES public.users(id);


--
-- Name: horses fk_owner; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.horses
    ADD CONSTRAINT fk_owner FOREIGN KEY (owner_id) REFERENCES public.users(id);


--
-- Name: users fk_status; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT fk_status FOREIGN KEY (status_id) REFERENCES public.status(id);


--
-- Name: contests hippodrome; Type: FK CONSTRAINT; Schema: public; Owner: server
--

ALTER TABLE ONLY public.contests
    ADD CONSTRAINT hippodrome FOREIGN KEY (hippodrome_id) REFERENCES public.hippodrome(id);


--
-- PostgreSQL database dump complete
--

